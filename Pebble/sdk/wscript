#
# This file is the compilation script for the built SDK for app developers to run
#

import waftools.objcopy as objcopy
import waftools.ldscript as ldscript
import waftools.inject_metadata as inject_metadata
import waftools.xcode_pebble
import time

top = '.'
out = 'build'

def options(opt):
    opt.load('gcc')
    opt.add_option('-d', '--debug', action='store_true', default=False, dest='debug', help='Build in debug mode')
    opt.add_option('-t', '--timestamp', dest='timestamp', help="Use a specific timestamp to label this package (ie, your repository's last commit time), defaults to time of build")

def configure(conf):
    CROSS_COMPILE_PREFIX = 'arm-none-eabi-'

    conf.env.AS = CROSS_COMPILE_PREFIX + 'gcc'
    conf.env.AR = CROSS_COMPILE_PREFIX + 'ar'
    conf.env.CC = CROSS_COMPILE_PREFIX + 'gcc'
    conf.env.LD = CROSS_COMPILE_PREFIX + 'ld'

    optimize_flag = '-Os'

    conf.load('gcc')

    conf.env.CFLAGS = [ '-std=c99',
                        '-mcpu=cortex-m3',
                        '-mthumb',
                        '-ffunction-sections',
                        '-fdata-sections',
                        '-g',
                        optimize_flag ]

    c_warnings = [ '-Wall',
                   '-Wextra',
                   '-Werror',
                   '-Wno-unused-parameter',
                   '-Wno-error=unused-function',
                   '-Wno-error=unused-variable' ]

    conf.env.append_value('CFLAGS', c_warnings)

    conf.env.LINKFLAGS = [ '-mcpu=cortex-m3',
                           '-mthumb',
                           '-Wl,--gc-sections',
                           '-Wl,--warn-common',
                           optimize_flag ]

    conf.env.SHLIB_MARKER = None
    conf.env.STLIB_MARKER = None

    if not conf.options.debug:
        conf.env.append_value('DEFINES', 'RELEASE')
    else:
        print "Debug enabled"

def build(bld):
    # TODO: find the actual required firmware
    req_fw = 1

    timestamp = bld.options.timestamp

    pbw_basename = 'app_' + str(timestamp) if timestamp else bld.path.name

    if timestamp is None:
        timestamp = int(time.time())

    sources = bld.path.ant_glob('src/*.c')

    bld.recurse('resources')

    elf_file = bld.path.get_bld().make_node('pebble-app.elf')

    bld.program(
        source       = sources,
        target       = elf_file,
        includes     = ['include', '.', 'build/src'],
        ldscript     = 'pebble_app.ld',
        stlibpath    = [bld.path.find_dir('lib').abspath()],
        stlib        = ['pebble'],
        # TODO: insert link to the wiki page I'm about to write about PIC and relocatable values
        cflags       = ['-fPIE'],
        linkflags    = ['-mcpu=cortex-m3', '-mthumb', '-fPIE', '-Wl,-Map,pebble-app.map,--emit-relocs'])

    raw_bin_file = bld.path.get_bld().make_node('pebble-app.raw.bin')
    bld(rule=objcopy.objcopy_bin, source=elf_file, target=raw_bin_file)


    def inject_data_rule(task):
        src_path = task.inputs[0].abspath()
        tgt_path = task.outputs[0].abspath()
        cp_result = task.exec_command('cp {} {}'.format(src_path, tgt_path))
        return cp_result or inject_metadata.do_inject(tgt_path)

    bin_file = bld.path.get_bld().make_node('pebble-app.bin')
    bld(rule=inject_data_rule, name='inject-metadata', source=raw_bin_file, target=bin_file)


    bundle_script = bld.path.find_node('tools/mkbundle.py')
    resource_pack = bld.path.get_bld().make_node('app_resources.pbpack')
    resource_map = bld.path.find_node('resources/src/resource_map.json')

    pbz_output = bld.path.get_bld().make_node(pbw_basename + '.pbw')

    bld(rule="python {script} --watchapp {bin_file} --watchapp-timestamp {timestamp} --req-fw {req_fw} --resources {resource_pack} --resource-map {resource_map} --resources-timestamp {timestamp} -o {pbz_file} -v".format(
            script=bundle_script.abspath(),
            bin_file=bin_file.abspath(),
            timestamp=timestamp,
            req_fw=req_fw,
            resource_pack=resource_pack.abspath(),
            resource_map=resource_map.abspath(),
            pbz_file=pbz_output.abspath()),
        source=[bundle_script, bin_file, resource_pack, resource_map],
        target=pbz_output)
