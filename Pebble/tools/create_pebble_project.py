#!/usr/bin/env python

import os
import sys
import uuid
import argparse

file_gitignore = """
# Ignore ignore files
.hgignore
.gitignore

# Ignore waf build tool generated files
.waf*
build
.lock-waf*

# Ignore linked files/directories
waf
wscript
tools
include
lib
pebble_app.ld

# Ignore other generated files
serial_dump.txt
"""

# When an empty resource map is required this can be used but...
FILE_DEFAULT_RESOURCE_MAP = """
{"friendlyVersion": "VERSION", "versionDefName": "VERSION", "media": []}
"""

# ...for the moment we need to have one with a dummy entry due to
# a bug that causes a hang when there's an empty resource map.
FILE_DUMMY_RESOURCE_MAP = """
{"friendlyVersion": "VERSION",
 "versionDefName": "VERSION",
 "media": [
	   {
	    "type":"raw",
	    "defName":"DUMMY",
	    "file":"resource_map.json"
	   }
	  ]
}
"""

# Note: Requires original directory layout to be maintained
TEMPLATE_APP_SRC_FILEPATH = os.path.join(os.path.dirname(os.path.abspath(__file__)),
                                         "..", "templates", "template_minimal", "src", "template_minimal.c")

def create_ignore_files(location):
    """
    """
    open(os.path.join(location, ".gitignore"), "w").write(file_gitignore)

    with open(os.path.join(location, ".hgignore"), "w") as the_file:
        the_file.write("# use glob syntax.\nsyntax: glob\n\n")
        the_file.write(file_gitignore)


def symlink(src, dst, force = False):
    try:
        if (force and os.path.lexists(dst)):
            os.remove(dst)
        os.symlink(src, dst)
    except OSError, e:
        print "Error symlinking #{} to #{}: #{}".format(src, dst, e)
        raise e
  
def create_symlinks(project_root, sdk_root, force = False):
    SDK_LINKS = ["waf", "wscript", "tools", "lib", "pebble_app.ld", "include"]

    for item_name in SDK_LINKS:
        symlink(os.path.join(sdk_root, item_name), os.path.join(project_root, item_name), force)

    symlink(os.path.join(sdk_root, os.path.join("resources", "wscript")),
               os.path.join(project_root, os.path.join("resources", "wscript")), force)

def generate_uuid_as_array():
    """
    Returns a freshly generated UUID value in string form formatted as
    a C array for inclusion in a template's "#define MY_UUID {...}"
    macro.
    """
    return ", ".join(["0x%02X" % ord(b) for b in uuid.uuid4().bytes])


# This is the dummy UUID value in the template file.
UUID_VALUE_TO_REPLACE="/* GENERATE YOURSELF USING `uuidgen` */ 0xBE, 0xEF, 0xBE, 0xEF, 0xBE, 0xEF, 0xBE, 0xEF, 0xBE, 0xEF, 0xBE, 0xEF, 0xBE, 0xEF, 0xBE, 0xEF"
def copy_app_template(template_source_filepath, destination_filepath):
    """
    Copies the supplied template file *and* replaces a dummy UUID
    value in it with a freshly generated value.
    """
    template_source_content = open(template_source_filepath).read()
    open(destination_filepath, "w").write(template_source_content.replace(UUID_VALUE_TO_REPLACE, generate_uuid_as_array(), 1))


def create_project(project_root, sdk_root):
    print "\nCreating project here:\n\n\t", project_root

    PROJECT_RESOURCES_SRC = os.path.join(project_root, os.path.join("resources","src"))

    PROJECT_SOURCE_FILES = os.path.join(project_root, "src")

    os.makedirs(project_root)
    os.makedirs(PROJECT_RESOURCES_SRC)
    os.makedirs(PROJECT_SOURCE_FILES)

    create_symlinks(project_root, sdk_root)

    open(os.path.join(PROJECT_RESOURCES_SRC, "resource_map.json"), "w").write(FILE_DUMMY_RESOURCE_MAP)

    create_ignore_files(project_root)

    # TODO: Use a template.c file instead?
    copy_app_template(TEMPLATE_APP_SRC_FILEPATH,
                      os.path.join(PROJECT_SOURCE_FILES, "%s.c" % (PROJECT_NAME)))


if __name__ == "__main__":

    parser = argparse.ArgumentParser(description="Create a new Pebble project with specified name and links to the Pebble SDK.")

    parser.add_argument("sdk_root", help="The path to the `sdk` directory.")

    parser.add_argument("project_name", help="The project is created in a directory of this name.")

    parser.add_argument("--symlink-only", help="Only create the symlinks to the SDK files/directories. (Use this when you have an existing project directory.)", action="store_true")
    
    parser.add_argument("--update-symlink", help="Updates the symlinks to point to a new SDK. (Use this when upgrading a project to a new SDK.)", action="store_true")

    args = parser.parse_args()

    SDK_ROOT = os.path.realpath(args.sdk_root)
    PROJECT_NAME = args.project_name
    PROJECT_ROOT = os.path.join(os.getcwd(), PROJECT_NAME)

    SYMLINK_ONLY = args.symlink_only or args.update_symlink
    UPDATE = args.update_symlink
    
    if not SYMLINK_ONLY:
        create_project(PROJECT_ROOT, SDK_ROOT)
    else:
        print "\nCreating symlinks here:\n\n\t", PROJECT_ROOT
        create_symlinks(PROJECT_ROOT, SDK_ROOT, UPDATE)

    print "\nNow run:\n\n\tcd %s\n\t./waf configure\n\t./waf build\n" % (PROJECT_NAME)
