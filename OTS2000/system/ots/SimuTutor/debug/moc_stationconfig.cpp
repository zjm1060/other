/****************************************************************************
** Meta object code from reading C++ file 'stationconfig.h'
**
** Created: Fri Jan 23 13:43:48 2015
**      by: The Qt Meta Object Compiler version 61 (Qt 4.5.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../src/stationconfig.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'stationconfig.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 61
#error "This file was generated using the moc from 4.5.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_CStationAddTable[] = {

 // content:
       2,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   12, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors

 // signals: signature, parameters, type, tag, flags
      18,   17,   17,   17, 0x05,
      33,   17,   17,   17, 0x05,

 // slots: signature, parameters, type, tag, flags
      48,   17,   17,   17, 0x0a,
      56,   17,   17,   17, 0x0a,
      64,   17,   17,   17, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_CStationAddTable[] = {
    "CStationAddTable\0\0insertRecord()\0"
    "modifyRecord()\0apply()\0enter()\0"
    "setLanguage(int)\0"
};

const QMetaObject CStationAddTable::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_CStationAddTable,
      qt_meta_data_CStationAddTable, 0 }
};

const QMetaObject *CStationAddTable::metaObject() const
{
    return &staticMetaObject;
}

void *CStationAddTable::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CStationAddTable))
        return static_cast<void*>(const_cast< CStationAddTable*>(this));
    if (!strcmp(_clname, "Ui_CStationAddTable"))
        return static_cast< Ui_CStationAddTable*>(const_cast< CStationAddTable*>(this));
    return QDialog::qt_metacast(_clname);
}

int CStationAddTable::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: insertRecord(); break;
        case 1: modifyRecord(); break;
        case 2: apply(); break;
        case 3: enter(); break;
        case 4: setLanguage((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 5;
    }
    return _id;
}

// SIGNAL 0
void CStationAddTable::insertRecord()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}

// SIGNAL 1
void CStationAddTable::modifyRecord()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}
static const uint qt_meta_data_CStationConfigTable[] = {

 // content:
       2,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   12, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors

 // slots: signature, parameters, type, tag, flags
      25,   20,   21,   20, 0x0a,
      44,   42,   20,   20, 0x0a,
      64,   20,   20,   20, 0x0a,
      77,   20,   20,   20, 0x0a,
      94,   20,   20,   20, 0x0a,
     114,   20,   20,   20, 0x0a,
     127,   20,   20,   20, 0x0a,
     143,   20,   20,   20, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_CStationConfigTable[] = {
    "CStationConfigTable\0\0int\0displayStation()\0"
    ",\0selectCell(int,int)\0delStation()\0"
    "openStationAdd()\0openStationModify()\0"
    "addStation()\0modifyStation()\0"
    "setLanguage(int)\0"
};

const QMetaObject CStationConfigTable::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_CStationConfigTable,
      qt_meta_data_CStationConfigTable, 0 }
};

const QMetaObject *CStationConfigTable::metaObject() const
{
    return &staticMetaObject;
}

void *CStationConfigTable::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CStationConfigTable))
        return static_cast<void*>(const_cast< CStationConfigTable*>(this));
    if (!strcmp(_clname, "Ui_CStationConfigTable"))
        return static_cast< Ui_CStationConfigTable*>(const_cast< CStationConfigTable*>(this));
    return QDialog::qt_metacast(_clname);
}

int CStationConfigTable::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: { int _r = displayStation();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 1: selectCell((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 2: delStation(); break;
        case 3: openStationAdd(); break;
        case 4: openStationModify(); break;
        case 5: addStation(); break;
        case 6: modifyStation(); break;
        case 7: setLanguage((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 8;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
