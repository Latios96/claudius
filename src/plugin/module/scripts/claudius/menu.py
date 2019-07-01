import pymel.core as pm


def create_menu():
    # Name of the global variable for the Maya window
    MainMayaWindow = pm.language.melGlobals['gMainWindow']

    # Build a menu and parent underthe Maya Window
    customMenu = pm.menu('Claudius', parent=MainMayaWindow)
    # Build a menu item and parent under the 'customMenu'
    pm.menuItem(label="Import Point Cloud", command="from claudius import conversion;conversion.do_convert()",
                parent=customMenu)


def remove_menu():
    pm.deleteUI('Claudius')
