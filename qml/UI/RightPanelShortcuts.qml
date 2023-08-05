import QtQuick 2.9

Item {
    focus: true
    signal viewButtonShortcutActivated(int index)
    signal menuItemShortcutActivated(int index)

    Shortcut {
        sequence: "A"
        onActivated: menuItemShortcutActivated(0)
    }
    Shortcut {
        sequence: "S"
        onActivated: menuItemShortcutActivated(1)
    }
    Shortcut {
        sequence: "D"
        onActivated: menuItemShortcutActivated(2)
    }
    Shortcut {
        sequence: "F"
        onActivated: menuItemShortcutActivated(3)
    }
    Shortcut {
        sequence: "G"
        onActivated: menuItemShortcutActivated(4)
    }
    Shortcut {
        sequence: "H"
        onActivated: menuItemShortcutActivated(5)
    }


    Shortcut {
        sequence: "Q"
        onActivated: viewButtonShortcutActivated(0)
    }
    Shortcut {
        sequence: "W"
        onActivated: viewButtonShortcutActivated(1)
    }
    Shortcut {
        sequence: "E"
        onActivated: viewButtonShortcutActivated(2)
    }
    Shortcut {
        sequence: "R"
        onActivated: viewButtonShortcutActivated(3)
    }
    Shortcut {
        sequence: "T"
        onActivated: viewButtonShortcutActivated(4)
    }
    Shortcut {
        sequence: "Y"
        onActivated: viewButtonShortcutActivated(5)
    }
}
