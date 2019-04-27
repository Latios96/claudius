import ConfigParser
import glob
import os
import shutil
import subprocess

from dotenv import load_dotenv


def read_current_version():
    config = ConfigParser.ConfigParser()
    config.read('.bumpversion.cfg')
    return config.get('bumpversion', 'current_version')


def should_release():
    current_version = read_current_version()
    current_tag = get_current_tag()
    if current_tag:
        if current_tag == "v" + current_version:
            return True
        else:
            print 'current tag and current version have to match: current tag: "{}", current version: "v{}"'.format(
                current_tag, current_version)
            return False


def get_current_tag():
    # return subprocess.check_output("git tag -l --points-at HEAD")
    return subprocess.check_output("git describe --tags").replace("\n", "")


def collect_files():
    return ['build/src/plugin/Release/claudius-*.zip']


def read_credentials():
    load_dotenv()


def deploy_to_one_drive(files, release=False):
    if release:
        print "doing release"
    else:
        print "deploy dev build"
    matched_files = set()
    for file_pattern in files:
        for f in glob.glob(file_pattern):
            matched_files.add(f)

    for matched_file in matched_files:
        target = os.path.join(r"C:\release_share\claudius_builds",
                              'releases' if release else 'dev_builds',
                              os.path.basename(matched_file))

        print "copy {} => {}".format(matched_file, target)
        shutil.copy(matched_file, target)


def upload_release():
    read_credentials()
    current_tag = get_current_tag()
    files = collect_files()

    # github_release.gh_release_create("Latios96/claudius", current_tag, name=current_tag, asset_pattern=files)
    # github_release.gh_asset_upload("Latios96/claudius", current_tag, asset_pattern=files)


if __name__ == '__main__':
    files = collect_files()
    deploy_to_one_drive(files, release=should_release())
