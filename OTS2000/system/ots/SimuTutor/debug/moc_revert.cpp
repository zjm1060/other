/****************************************************************************
** Meta object code from reading C++ file 'revert.h'
**
** Created: Fri Jan 23 13:44:05 2015
**      by: The Qt Meta Object Compiler version 61 (Qt 4.5.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../src/revert.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'revert.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 61
#error "This file was generated using the moc from 4.5.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_CRevertAddTable[] = {

 // content:
       2,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   12, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors

 // signals: signature, parameters, type, tag, flags
      17,   16,   16,   16, 0x05,
      32,   16,   16,   16, 0x05,

 // slots: signature, parameters, type, tag, flags
      47,   16,   16,   16, 0x0a,
      64,   16,   16,   16, 0x08,
      72,   16,   16,   16, 0x08,
      80,   16,   16,   16, 0x08,
     101,   97,   16,   16, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_CRevertAddTable[] = {
    "CRevertAddTable\0\0insertRecord()\0"
    "modifyRecord()\0setLanguage(int)\0apply()\0"
    "enter()\0displayStation()\0stn\0"
    "displayDev(int)\0"
};

const QMetaObject CRevertAddTable::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_CRevertAddTable,
      qt_meta_data_CRevertAddTable, 0 }
};

const QMetaObject *CRevertAddTable::metaObject() const
{
    return &staticMetaObject;
}

void *CRevertAddTable::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CRevertAddTable))
        return static_cast<void*>(const_cast< CRevertAddTable*>(this));
    if (!strcmp(_clname, "Ui_CRevertAddTable"))
        return static_cast< Ui_CRevertAddTable*>(const_cast< CRevertAddTable*>(this));
    return QDialog::qt_metacast(_clname);
}

int CRevertAddTable::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: insertRecord(); break;
        case 1: modifyRecord(); break;
        case 2: setLanguage((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: apply(); break;
        case 4: enter(); break;
        case 5: displayStation(); break;
        case 6: displayDev((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 7;
    }
    return _id;
}

// SIGNAL 0
void CRevertAddTable::insertRecord()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}

// SIGNAL 1
void CRevertAddTable::modifyRecord()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}
static const uint qt_meta_data_CRevertTable[] = {

 // content:
       2,       // revision
       0,       // classname
       0,    0, // classinfo
      11,   12, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors

 // slots: signature, parameters, type, tag, flags
      18,   13,   14,   13, 0x0a,
      36,   34,   13,   13, 0x0a,
      56,   13,   13,   13, 0x0a,
      68,   13,   13,   13, 0x0a,
      84,   13,   13,   13, 0x0a,
     103,   13,   13,   13, 0x0a,
     115,   13,   13,   13, 0x0a,
     130,   13,   13,   13, 0x0a,
     143,   13,   13,   13, 0x0a,
     162,   34,   13,   13, 0x0a,
     185,   13,   13,   13, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_CRevertTable[] = {
    "CRevertTable\0\0int\0displayRevert()\0,\0"
    "selectCell(int,int)\0delRevert()\0"
    "openRevertAdd()\0openRevertModify()\0"
    "addRevert()\0modifyRevert()\0openStnDev()\0"
    "displayUpdate(int)\0displayUpdate(int,int)\0"
    "setLanguage(int)\0"
};

const QMetaObject CRevertTable::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_CRevertTable,
      qt_meta_data_CRevertTable, 0 }
};

const QMetaObject *CRevertTable::metaObject() const
{
    return &staticMetaObject;
}

void *CRevertTable::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CRevertTable))
        return static_cast<void*>(const_cast< CRevertTable*>(this));
    if (!strcmp(_clname, "Ui_CRevertTable"))
        return static_cast< Ui_CRevertTable*>(const_cast< CRevertTable*>(this));
    return QDialog::qt_metacast(_clname);
}

int CRevertTable::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: { int _r = displayRevert();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 1: selectCell((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 2: delRevert(); break;
        case 3: openRevertAdd(); break;
        case 4: openRevertModify(); break;
        case 5: addRevert(); break;
        case 6: modifyRevert(); break;
        case 7: openStnDev(); break;
        case 8: displayUpdate((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 9: displayUpdate((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 10: setLanguage((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 11;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
