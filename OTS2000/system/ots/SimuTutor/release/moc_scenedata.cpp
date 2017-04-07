/****************************************************************************
** Meta object code from reading C++ file 'scenedata.h'
**
** Created: Thu Jan 15 10:27:19 2015
**      by: The Qt Meta Object Compiler version 61 (Qt 4.5.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../src/scenedata.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'scenedata.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 61
#error "This file was generated using the moc from 4.5.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_CSceneDataAddTable[] = {

 // content:
       2,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   12, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors

 // signals: signature, parameters, type, tag, flags
      20,   19,   19,   19, 0x05,
      35,   19,   19,   19, 0x05,

 // slots: signature, parameters, type, tag, flags
      50,   19,   19,   19, 0x0a,
      58,   19,   19,   19, 0x0a,
      66,   19,   19,   19, 0x0a,
      82,   19,   19,   19, 0x0a,
     104,   19,   19,   19, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_CSceneDataAddTable[] = {
    "CSceneDataAddTable\0\0insertRecord()\0"
    "modifyRecord()\0apply()\0enter()\0"
    "openPointName()\0setPointName(QString)\0"
    "setLanguage(int)\0"
};

const QMetaObject CSceneDataAddTable::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_CSceneDataAddTable,
      qt_meta_data_CSceneDataAddTable, 0 }
};

const QMetaObject *CSceneDataAddTable::metaObject() const
{
    return &staticMetaObject;
}

void *CSceneDataAddTable::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CSceneDataAddTable))
        return static_cast<void*>(const_cast< CSceneDataAddTable*>(this));
    if (!strcmp(_clname, "Ui_CSceneDataAddTable"))
        return static_cast< Ui_CSceneDataAddTable*>(const_cast< CSceneDataAddTable*>(this));
    return QDialog::qt_metacast(_clname);
}

int CSceneDataAddTable::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
        case 4: openPointName(); break;
        case 5: setPointName((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 6: setLanguage((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 7;
    }
    return _id;
}

// SIGNAL 0
void CSceneDataAddTable::insertRecord()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}

// SIGNAL 1
void CSceneDataAddTable::modifyRecord()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}
static const uint qt_meta_data_CSceneDataTable[] = {

 // content:
       2,       // revision
       0,       // classname
       0,    0, // classinfo
      13,   12, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors

 // slots: signature, parameters, type, tag, flags
      17,   16,   16,   16, 0x0a,
      36,   16,   16,   16, 0x0a,
      58,   16,   16,   16, 0x0a,
      73,   16,   16,   16, 0x0a,
      88,   16,   16,   16, 0x0a,
     106,   16,   16,   16, 0x0a,
     127,   16,   16,   16, 0x0a,
     140,   16,   16,   16, 0x0a,
     156,   16,   16,   16, 0x0a,
     173,   16,   16,   16, 0x0a,
     188,   16,   16,   16, 0x0a,
     205,   16,   16,   16, 0x0a,
     218,   16,   16,   16, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_CSceneDataTable[] = {
    "CSceneDataTable\0\0openSceneDataAdd()\0"
    "openSceneDataModify()\0addSceneData()\0"
    "delSceneData()\0modifySceneData()\0"
    "setDataSourceModel()\0displayStn()\0"
    "displayDev(int)\0displayType(int)\0"
    "setFilter(int)\0searchFromName()\0"
    "displayAll()\0setLanguage(int)\0"
};

const QMetaObject CSceneDataTable::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_CSceneDataTable,
      qt_meta_data_CSceneDataTable, 0 }
};

const QMetaObject *CSceneDataTable::metaObject() const
{
    return &staticMetaObject;
}

void *CSceneDataTable::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CSceneDataTable))
        return static_cast<void*>(const_cast< CSceneDataTable*>(this));
    if (!strcmp(_clname, "Ui_CSceneDataTable"))
        return static_cast< Ui_CSceneDataTable*>(const_cast< CSceneDataTable*>(this));
    return QDialog::qt_metacast(_clname);
}

int CSceneDataTable::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: openSceneDataAdd(); break;
        case 1: openSceneDataModify(); break;
        case 2: addSceneData(); break;
        case 3: delSceneData(); break;
        case 4: modifySceneData(); break;
        case 5: setDataSourceModel(); break;
        case 6: displayStn(); break;
        case 7: displayDev((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 8: displayType((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 9: setFilter((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 10: searchFromName(); break;
        case 11: displayAll(); break;
        case 12: setLanguage((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 13;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
