import subprocess
import github_release
from dotenv import load_dotenv
import ConfigParser

def read_current_version():
    config = ConfigParser.ConfigParser()
    config.read('.bumpversion.cfg')
    return config.get('bumpversion', 'current_version')


def should_release():
    current_version = read_current_version()
    current_tag = get_current_tag()
    if current_tag:
        if current_tag == "v"+current_version:
            return True
        else:
            print "current tag and current version have to match: current tag: {}, current version: {}".format(current_tag, current_version)
            return False


def get_current_tag():
    # return subprocess.check_output("git tag -l --points-at HEAD")
    return subprocess.check_output("git describe --tags")


def collect_files():
    return ['build/src/Release/claudius-*.zip']


def read_credentials():
    load_dotenv()


def upload_release():
    read_credentials()
    current_tag = get_current_tag()
    files = collect_files()

    github_release.gh_release_create("Latios96/claudius", current_tag, name=current_tag, asset_pattern=files)
    github_release.gh_asset_upload("Latios96/claudius", current_tag, asset_pattern=files)


if __name__ == '__main__':
    if should_release():
        print "we do upload a release"
        upload_release()
    else:
        print "skipping release upload"
