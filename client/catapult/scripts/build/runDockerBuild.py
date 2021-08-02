import argparse
import sys
from pathlib import Path

from BasicBuildManager import BasicBuildManager
from environment import EnvironmentManager
from process import ProcessManager


def root_directory(directory_name):
    root_directory_prefix = 'c:\\' if 'win32' == sys.platform else '/'
    return root_directory_prefix + directory_name


CCACHE_ROOT = root_directory('jenkins_cache/ccache')
CONAN_ROOT = root_directory('jenkins_cache/conan')

SRC_DIR = Path('catapult-src').resolve()
OUTPUT_DIR = Path('output')
BINARIES_DIR = OUTPUT_DIR / 'binaries'

class OptionsManager(BasicBuildManager):
    def __init__(self, args):
        super().__init__(args.compiler_configuration, args.build_configuration)
        self.operating_system = args.operating_system

    @property
    def image_type(self):
        return 'release' if self.is_release else 'test'

    @property
    def version(self):
        version = self.image_type
        if self.enable_diagnostics:
            version += '-diagnostics'

        return '-'.join([version] + self.sanitizers + [self.architecture])

    @property
    def build_base_image_name(self):
        if self.use_conan:
            name_parts = [self.operating_system, self.versioned_compiler, 'conan']
        else:
            name_parts = [self.operating_system, self.compilation_friendly_name]

        return 'symbolplatform/symbol-server-build-base:{}'.format('-'.join(name_parts))

    @property
    def prepare_base_image_name(self):
        return 'symbolplatform/symbol-server-test-base:{}'.format(self.operating_system)

    @property
    def ccache_path(self):
        return Path(CCACHE_ROOT) / ('release' if self.is_release else 'all')

    @property
    def conan_path(self):
        if self.is_clang:
            return Path(CONAN_ROOT) / 'clang'

        elif self.is_msvc:
            return Path('d:/msvc')

        return Path(CONAN_ROOT) / 'gcc'

    def docker_run_settings(self):
        settings = [] if self.is_msvc else [
            ('CC', self.compiler.c),
            ('CXX', self.compiler.cpp),
            ('CCACHE_DIR', '/ccache')
        ]

        return ['--env={}={}'.format(key, value) for key, value in sorted(settings)]


    def get_volume_mappings(self):
        mappings = [
            (SRC_DIR, root_directory('catapult-src')),
            (BINARIES_DIR, root_directory('binaries')),
            (self.conan_path, root_directory('conan'))
        ]

        if not self.is_msvc:
            mappings.append((self.ccache_path, root_directory('ccache')))

        return ['--volume={}:{}'.format(str(key), value) for key, value in sorted(mappings)]


def create_docker_run_command(options, compiler_configuration_filepath, build_configuration_filepath, user):
    docker_run_settings = options.docker_run_settings()
    volume_mappings = options.get_volume_mappings()

    docker_args = [
        'docker', 'run',
        '--rm'
    ]

    if 'win32' != sys.platform:
        docker_args.append('--user={}'.format(user))

    docker_args.extend(docker_run_settings)
    docker_args.extend(volume_mappings)

    docker_args.extend([
        options.build_base_image_name,
        'python3', '/catapult-src/scripts/build/runDockerBuildInnerBuild.py',
        # assume paths are relative to workdir
        '--compiler-configuration=/{}'.format(compiler_configuration_filepath),
        '--build-configuration=/{}'.format(build_configuration_filepath)
    ])

    return docker_args


def cleanup_directories(environment_manager, ccache_root_directory, conan_root_directory):
    environment_manager.rmtree(OUTPUT_DIR)
    environment_manager.mkdirs(BINARIES_DIR)

    environment_manager.mkdirs(ccache_root_directory, exist_ok=True)
    environment_manager.mkdirs(conan_root_directory, exist_ok=True)


def prepare_docker_image(process_manager, container_id, prepare_replacements):
    destination_image_label = prepare_replacements['destination_image_label']
    cid_filepath = Path('{}.cid'.format(destination_image_label))
    if not container_id:
        if cid_filepath.exists():
            cid_filepath.unlink()

    build_disposition = prepare_replacements['build_disposition']
    disposition_to_repository_map = {
        'tests': 'symbol-server-test',
        'private': 'symbol-server-private',
        'public': 'symbol-server'
    }
    destination_repository = disposition_to_repository_map[build_disposition]

    print('*** volume paths prior to run')
    print('SRC_DIR:       {}'.format(SRC_DIR))
    print('OUTPUT_DIR:    {}'.format(OUTPUT_DIR.resolve()))

    destination_image_name = 'symbolplatform/{}:{}'.format(destination_repository, destination_image_label)
    process_manager.dispatch_subprocess([
        'docker', 'run',
        '--cidfile={}'.format(cid_filepath),
        '--volume={}:{}'.format(SRC_DIR / 'scripts' / 'build', root_directory('scripts')),
        '--volume={}:{}'.format(str(OUTPUT_DIR.resolve()), root_directory('data')),
        'registry.hub.docker.com/{}'.format(prepare_replacements['base_image_name']),
        'python3', '/scripts/runDockerBuildInnerPrepare.py',
        '--disposition={}'.format(build_disposition)
    ])

    if not container_id:
        with open(cid_filepath, 'rt') as cid_infile:
            container_id = cid_infile.read()

    process_manager.dispatch_subprocess(['docker', 'commit', container_id, destination_image_name])


def main():
    global OUTPUT_DIR, BINARIES_DIR

    parser = argparse.ArgumentParser(description='catapult project build generator')
    parser.add_argument('--compiler-configuration', help='path to compiler configuration yaml', required=True)
    parser.add_argument('--build-configuration', help='path to build configuration yaml', required=True)
    parser.add_argument('--operating-system', help='operating system', required=True)
    parser.add_argument('--user', help='docker user', required=True)
    parser.add_argument('--destination-image-label', help='docker destination image label', required=True)
    parser.add_argument('--dry-run', help='outputs desired commands without running them', action='store_true')
    parser.add_argument('--base-image-names-only', help='only output the base image names', action='store_true')
    args = parser.parse_args()

    options = OptionsManager(args)

    if args.base_image_names_only:
        print(options.build_base_image_name)
        print(options.prepare_base_image_name)
        return

    environment_manager = EnvironmentManager(args.dry_run)
    environment_manager.rmtree(OUTPUT_DIR)
    environment_manager.mkdirs(BINARIES_DIR)
    environment_manager.mkdirs(options.ccache_path / 'tmp', exist_ok=True)
    environment_manager.mkdirs(options.conan_path, exist_ok=True)

    OUTPUT_DIR = OUTPUT_DIR.resolve()
    BINARIES_DIR = BINARIES_DIR.resolve()

    print('*** *** *** *** *** ***')
    print('SRC_DIR:       {}'.format(SRC_DIR))
    print('OUTPUT_DIR:    {}'.format(OUTPUT_DIR))
    print('BINARIES_DIR:  {}'.format(BINARIES_DIR))
    print('*** *** *** *** *** ***')

    docker_run = create_docker_run_command(options, args.compiler_configuration, args.build_configuration, args.user)

    print('building project')

    process_manager = ProcessManager(args.dry_run)

    if options.is_msvc:
        process_manager.dispatch_subprocess(['cmd', '/c', 'dir', 'd:\\msvc'])
        process_manager.dispatch_subprocess(['cmd', '/c', 'dir', 'd:\\msvc\\conan'])
        process_manager.dispatch_subprocess(['cmd', '/c', 'dir', 'd:\\msvc\\conan\\.conan'])

    return_code = process_manager.dispatch_subprocess(docker_run)
    if return_code:
        sys.exit(return_code)

    print('copying files')

    environment_manager.chdir(OUTPUT_DIR)

    for folder_name in ['scripts', 'seed', 'resources']:
        environment_manager.copy_tree_with_symlinks(SRC_DIR / folder_name, folder_name)

    environment_manager.chdir(SRC_DIR)

    print('building docker image')

    container_id = '<dry_run_container_id>' if args.dry_run else None
    prepare_docker_image(process_manager, container_id, {
        'base_image_name': options.prepare_base_image_name,
        'destination_image_label': args.destination_image_label,
        'build_disposition': options.build_disposition
    })


if __name__ == '__main__':
    main()
