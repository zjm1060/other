/****************************************************************************
** Meta object code from reading C++ file 'privilegemanager.h'
**
** Created: Fri Jan 23 13:44:07 2015
**      by: The Qt Meta Object Compiler version 61 (Qt 4.5.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../src/privilegemanager.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'privilegemanager.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 61
#error "This file was generated using the moc from 4.5.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_CPrivilegeMananger[] = {

 // content:
       2,       // revision
       0,       // classname
       0,    0, // classinfo
      13,   12, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors

 // signals: signature, parameters, type, tag, flags
      20,   19,   19,   19, 0x05,

 // slots: signature, parameters, type, tag, flags
      40,   19,   19,   19, 0x08,
      52,   19,   19,   19, 0x08,
      64,   19,   19,   19, 0x08,
      79,   19,   19,   19, 0x08,
      90,   19,   19,   19, 0x08,
     102,   19,   19,   19, 0x08,
     124,   19,   19,   19, 0x08,
     145,   19,   19,   19, 0x08,
     171,   19,   19,   19, 0x08,
     201,   19,   19,   19, 0x08,
     215,   19,   19,   19, 0x08,
     242,   19,   19,   19, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_CPrivilegeMananger[] = {
    "CPrivilegeMananger\0\0sigPrivChanged(int)\0"
    "selectAll()\0selectOff()\0selectInvert()\0"
    "privCopy()\0privPaste()\0popupUserMenu(QPoint)\0"
    "privModeChanged(int)\0showUserPriv(QModelIndex)\0"
    "setPrivMask(QListWidgetItem*)\0"
    "saveAllPriv()\0userFilterChanged(QString)\0"
    "setLanguage(int)\0"
};

const QMetaObject CPrivilegeMananger::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_CPrivilegeMananger,
      qt_meta_data_CPrivilegeMananger, 0 }
};

const QMetaObject *CPrivilegeMananger::metaObject() const
{
    return &staticMetaObject;
}

void *CPrivilegeMananger::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CPrivilegeMananger))
        return static_cast<void*>(const_cast< CPrivilegeMananger*>(this));
    if (!strcmp(_clname, "Ui_CPrivilegeMananger"))
        return static_cast< Ui_CPrivilegeMananger*>(const_cast< CPrivilegeMananger*>(this));
    return QDialog::qt_metacast(_clname);
}

int CPrivilegeMananger::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: sigPrivChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: selectAll(); break;
        case 2: selectOff(); break;
        case 3: selectInvert(); break;
        case 4: privCopy(); break;
        case 5: privPaste(); break;
        case 6: popupUserMenu((*reinterpret_cast< const QPoint(*)>(_a[1]))); break;
        case 7: privModeChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 8: showUserPriv((*reinterpret_cast< const QModelIndex(*)>(_a[1]))); break;
        case 9: setPrivMask((*reinterpret_cast< QListWidgetItem*(*)>(_a[1]))); break;
        case 10: saveAllPriv(); break;
        case 11: userFilterChanged((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 12: setLanguage((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 13;
    }
    return _id;
}

// SIGNAL 0
void CPrivilegeMananger::sigPrivChanged(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
