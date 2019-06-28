import glob
import os
import pymel.core as pm
import maya.cmds as cmds
import time
from PySide2 import QtWidgets

MAX = 13000000


def pretty_time(seconds):
    minute_str = str(seconds / 60)
    seconds_str = str(seconds % 60)
    return "{}m {}s".format(minute_str.zfill(2), seconds_str)


def create_output_template(dir, original_file_path):
    name, ext = os.path.splitext(original_file_path)
    return os.path.join(dir, name + "_{}" + ext)


def export_node_to_ass(claudius_node, target_path):
    old_sel = pm.selected()

    pm.select(claudius_node)
    cmds.file(target_path, force=True,
              options="-expandProcedurals;-shadowLinks 1;-mask 6399;-lightLinks 1;-boundingBox",
              typ="ASS Export",
              pr=True,
              es=True)

    pm.select(old_sel)


def chunks_to_ass(chunk_files):
    claudius_node = pm.createNode('ClaudiusVisualizer')
    claudius_node.renderWithColor.set(True)

    ass_files = []

    for i, chunk_file in enumerate(chunk_files):
        print "Converting chunk {}".format(i)
        claudius_node.particleFile.set(chunk_file)
        target_path = os.path.splitext(chunk_file)[0] + ".ass"
        export_node_to_ass(claudius_node, target_path)
        ass_files.append(target_path)

    # NOTE: ClaudiusVisualizer always has a transform as a parent. If we only delete
    # the ClaudiusVisualizer node, we have an unused transform in our scene.
    pm.delete(claudius_node.parent(0))

    return ass_files


def process_file(input_path, output_template):
    pm.loadPlugin('mtoa')
    file_counter = 0
    counter = 0
    lines = []
    start = time.time()
    first_line = True

    written_files = []

    with open(input_path, 'r') as f:
        for line in f:
            if first_line:
                first_line = False
                continue
            if counter == MAX:
                file_name = output_template.format(file_counter)
                written_files.append((file_name))
                with open(output_template.format(file_counter), "w") as chunk_file:
                    print "writing chunk {}".format(file_counter)
                    chunk_file.write(str(len(lines)) + "\n")
                    chunk_file.writelines(lines)
                    lines = []
                    counter = 0
                    file_counter += 1
            lines.append(line)
            counter += 1

    file_name = output_template.format(file_counter)
    written_files.append((file_name))
    with open(file_name, "w") as chunk_file:
        print "writing chunk {}".format(file_counter)
        chunk_file.write(str(len(lines)) + "\n")
        chunk_file.writelines(lines)

    print "Time to split pts into chunks: {}".format(pretty_time(time.time() - start))

    ass_start = time.time()
    ass_files = chunks_to_ass(written_files)

    print "Time to create .ass files {}".format(pretty_time(ass_start - start))
    print "Total time: {}".format(pretty_time(time.time() - start))
    return ass_files


def import_ass_files(ass_files):
    for i, ass_files in enumerate(ass_files):
        print "Importing chunk {}".format(i)
        cmds.file(ass_files, i=True)


def do_convert():
    input_path = QtWidgets.QFileDialog.getOpenFileName()[0] # todo check for .pts files
    if input_path:
        output_template = create_output_template(os.path.dirname(input_path), input_path)
        ass_files = process_file(input_path, output_template)
        import_ass_files(ass_files)
