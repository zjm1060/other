/****************************************************************************
** Meta object code from reading C++ file 'cmdthread.h'
**
** Created: Wed Jan 14 10:22:32 2015
**      by: The Qt Meta Object Compiler version 61 (Qt 4.5.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../src/cserver/cmdthread.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'cmdthread.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 61
#error "This file was generated using the moc from 4.5.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_CmdThread[] = {

 // content:
       2,       // revision
       0,       // classname
       0,    0, // classinfo
      12,   12, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors

 // slots: signature, parameters, type, tag, flags
      11,   10,   10,   10, 0x0a,
      21,   10,   10,   10, 0x0a,
      34,   10,   10,   10, 0x0a,
      51,   10,   10,   10, 0x0a,
      70,   10,   10,   10, 0x0a,
      87,   10,   10,   10, 0x0a,
     107,   10,   10,   10, 0x0a,
     125,   10,   10,   10, 0x0a,
     141,   10,   10,   10, 0x0a,
     157,   10,   10,   10, 0x0a,
     179,   10,   10,   10, 0x0a,
     195,   10,   10,   10, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_CmdThread[] = {
    "CmdThread\0\0newData()\0newCommand()\0"
    "cmdMouseMoveTo()\0cmdMouseLeftDown()\0"
    "cmdMouseLeftUp()\0cmdMouseRightDown()\0"
    "cmdMouseRightUp()\0cmdMouseWheel()\0"
    "cmdScreenSize()\0cmdMouseDoubleClick()\0"
    "cmdKeyPressed()\0cmdKeyReleased()\0"
};

const QMetaObject CmdThread::staticMetaObject = {
    { &QThread::staticMetaObject, qt_meta_stringdata_CmdThread,
      qt_meta_data_CmdThread, 0 }
};

const QMetaObject *CmdThread::metaObject() const
{
    return &staticMetaObject;
}

void *CmdThread::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CmdThread))
        return static_cast<void*>(const_cast< CmdThread*>(this));
    return QThread::qt_metacast(_clname);
}

int CmdThread::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QThread::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: newData(); break;
        case 1: newCommand(); break;
        case 2: cmdMouseMoveTo(); break;
        case 3: cmdMouseLeftDown(); break;
        case 4: cmdMouseLeftUp(); break;
        case 5: cmdMouseRightDown(); break;
        case 6: cmdMouseRightUp(); break;
        case 7: cmdMouseWheel(); break;
        case 8: cmdScreenSize(); break;
        case 9: cmdMouseDoubleClick(); break;
        case 10: cmdKeyPressed(); break;
        case 11: cmdKeyReleased(); break;
        default: ;
        }
        _id -= 12;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
