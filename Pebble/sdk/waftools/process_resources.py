import json
import os, sys
import time
import re
import waflib

def gen_resource_deps(bld,
                      map_node,
                      pack_node,
                      id_header_node,
                      resource_header_path,
                      font_key_header_node=None,
                      font_key_table_node=None,
                      font_key_include_path=None,
                      timestamp=None):
    """
    Creates tasks to generate the resources described in the map file,
    Assumes that the map file is in the resource src directory
    """

    res_src_node = map_node.parent
    bitmap_script = bld.path.parent.find_node('tools/bitmapgen.py')
    font_script = bld.path.parent.find_node('tools/font/fontgen.py')

    pack_entries = []
    font_keys = []

    def deploy_generator(entry):
        res_type = entry["type"]
        def_name = entry["defName"]
        input_file = str(entry["file"])

        if res_type == "raw":
            output_node = res_src_node.get_bld().make_node(input_file)
            input_node = res_src_node.find_node(input_file)

            pack_entries.append( (output_node, def_name) )
            bld(rule = "cp ${SRC} ${TGT}",
                source = input_node,
                target = output_node)

        elif res_type == "png":
            output_pbi = res_src_node.get_bld().make_node(input_file + '.pbi' )
            input_png = res_src_node.find_node(input_file)

            pack_entries.append( (output_pbi, def_name) )
            bld(rule = "python {} pbi {} {}".format(bitmap_script.abspath(), input_png.abspath(), output_pbi.abspath()),
                source = [input_png, bitmap_script],
                target = output_pbi)

        elif res_type == "png-trans":
            output_white_pbi = res_src_node.get_bld().make_node(input_file + '.white.pbi' )
            output_black_pbi = res_src_node.get_bld().make_node(input_file + '.black.pbi' )
            input_png = res_src_node.find_node(input_file)

            pack_entries.append( (output_white_pbi, def_name + "_WHITE") )
            pack_entries.append( (output_black_pbi, def_name + "_BLACK") )

            bld(rule = "python {} white_trans_pbi {} {}".format(bitmap_script.abspath(), input_png.abspath(), output_white_pbi.abspath()),
                source = [input_png, bitmap_script],
                target = output_white_pbi)
            bld(rule = "python {} black_trans_pbi {} {}".format(bitmap_script.abspath(), input_png.abspath(), output_black_pbi.abspath()),
                source = [input_png, bitmap_script],
                target = output_black_pbi)

        elif res_type == "font":
            output_pfo = res_src_node.get_bld().make_node(input_file + '.' + str(def_name) + '.pfo')
            input_ttf = res_src_node.find_node(input_file)
            m = re.search('([0-9]+)', def_name)
            if m == None:
                if def_name != 'FONT_FALLBACK':
                    raise ValueError('Font {0}: no height found in def name\n'.format(self.def_name))
                height = 14
            else:
                height = int(m.group(0))

            pack_entries.append( (output_pfo, def_name) )
            font_keys.append(def_name)
            if 'trackingAdjust' in entry:
                trackingAdjustArg = '--tracking %i' % entry['trackingAdjust']
            else:
                trackingAdjustArg = ''
            if 'characterRegex' in entry:
                characterRegexArg = '--filter "%s"' % entry['characterRegex'].encode('utf-8')
            else:
                characterRegexArg = ''
            bld(rule = "python {} pfo {} {} {} {} {}".format(font_script.abspath(),
                                                                      height,
                                                                      trackingAdjustArg,
                                                                      characterRegexArg,
                                                                      input_ttf.abspath(),
                                                                      output_pfo.abspath()),
                source = [input_ttf, font_script],
                target = output_pfo)
        else:
            waflib.Logs.error("Error Generating Resources: File: " + input_file + " has specified invalid type: " + res_type)
            waflib.Logs.error("Must be one of (raw, png, png-trans, font)")
            waflib.Logs.error("Error in " + map_node.abspath())
            raise waflib.Errors.WafError("Generating resources failed")

    if timestamp == None:
        timestamp = int(time.time())

    with open(map_node.abspath()) as map_file:
        map_data = json.loads(map_file.read())

    readable_version = map_data["friendlyVersion"]
    version_def_name = map_data['versionDefName']

    for res in map_data["media"]:
        deploy_generator(res)

    # concat all of the pack entries
    manifest_node = res_src_node.get_bld().make_node(os.path.basename(pack_node.relpath()) + '.manifest')
    table_node = res_src_node.get_bld().make_node(os.path.basename(pack_node.relpath()) + '.table')
    data_node = res_src_node.get_bld().make_node(os.path.basename(pack_node.relpath()) + '.data')

    md_script = bld.path.parent.find_node('tools/pbpack_meta_data.py')
    resource_code_script = bld.path.parent.find_node('tools/generate_resource_code.py')

    data_sources = []

    cat_string = "cat"
    table_string = "python {} table {}".format(md_script.abspath(), table_node.abspath())
    resource_header_string = "python {script} resource_header {output_header} {version_def_name} {readable_version} {timestamp} {resource_include} {data_file}".format(
        script=resource_code_script.abspath(),
        output_header=id_header_node.abspath(),
        version_def_name=version_def_name,
        readable_version=readable_version,
        timestamp=timestamp,
        resource_include=resource_header_path,
        data_file=data_node.abspath())

    for entry in pack_entries:
        cat_string += ' ' + entry[0].abspath()
        data_sources.append(entry[0])
        table_string += ' ' + entry[0].abspath()
        resource_header_string += ' ' + str(entry[0].abspath()) + ' ' + str(entry[1])

    cat_string += ' > ' + data_node.abspath()

    bld(rule = cat_string,
        source = data_sources,
        target = data_node)

    bld(rule = table_string,
        source = data_sources + [md_script],
        target = table_node)


    bld(rule = "python {script} manifest {output_file} {num_files} {timestamp} {readable_version} {data_chunk_file}".format(
            script = md_script.abspath(),
            output_file = manifest_node.abspath(),
            num_files = len(pack_entries),
            timestamp = timestamp,
            readable_version = readable_version,
            data_chunk_file = data_node.abspath()),
        source = [data_node, md_script],
        target = manifest_node)

    # make the actual .pbpack
    bld(rule = "cat {} {} {} > {}".format(manifest_node.abspath(), table_node.abspath(), data_node.abspath(), pack_node.abspath()),
        source = [manifest_node, table_node, data_node],
        target = pack_node)

    # resource ids header
    bld(rule=resource_header_string,
        source = data_sources + [resource_code_script, data_node],
        target = id_header_node)

    # font definition files
    if font_key_header_node and font_key_table_node and font_key_include_path:
        key_list_string = " ".join(font_keys)

        bld(rule="python {script} font_key_header {font_key_header} {key_list}".format(
                script=resource_code_script.abspath(),
                font_key_header=font_key_header_node.abspath(),
                key_list=key_list_string),
            source=resource_code_script,
            target=font_key_header_node)

        bld(rule="python {script} font_key_table {font_key_table} {resource_id_header} {font_key_header} {key_list}".format(
                script=resource_code_script.abspath(),
                font_key_table=font_key_table_node.abspath(),
                resource_id_header=resource_header_path,
                font_key_header=font_key_include_path,
                key_list=key_list_string),
            source=resource_code_script,
            target=font_key_table_node)
