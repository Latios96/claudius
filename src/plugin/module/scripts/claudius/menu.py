import pymel.core as pm

from claudius import conversion

WINDOW_TITLE = 'Import Point Cloud'


def create_menu():
    # Name of the global variable for the Maya window
    MainMayaWindow = pm.language.melGlobals['gMainWindow']

    # Build a menu and parent underthe Maya Window
    customMenu = pm.menu('Claudius', parent=MainMayaWindow)
    # Build a menu item and parent under the 'customMenu'
    pm.menuItem(label="Import Point Cloud", command=conversion.do_convert,
                parent=customMenu)
    pm.menuItem(label="Import Point Cloud", command=show_options_menu,
                parent=customMenu, optionBox=True)


def show_options_menu(arg):

    window = pm.window(WINDOW_TITLE, width=200)
    pm.rowColumnLayout(numberOfColumns=2, columnAttach=(1, 'right', 0), columnWidth=[(1, 100), (2, 250)])
    pm.text('')
    with_colors = pm.checkBox('Import with colors')
    pm.text(label='Particles per chunk')
    chunk_size = pm.textField(text='13000000')
    pm.text(label='')
    pm.button('Import Point Cloud',
              command=lambda x: conversion.do_convert(pm.checkBox(with_colors, query=True, value=True),
                                                      int(pm.textField(chunk_size, query=True, text=True))))

    pm.showWindow()


def remove_menu():
    pm.deleteUI('Claudius')
