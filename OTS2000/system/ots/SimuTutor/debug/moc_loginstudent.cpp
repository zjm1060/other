/****************************************************************************
** Meta object code from reading C++ file 'loginstudent.h'
**
** Created: Fri Jan 23 13:44:13 2015
**      by: The Qt Meta Object Compiler version 61 (Qt 4.5.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../src/loginstudent.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'loginstudent.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 61
#error "This file was generated using the moc from 4.5.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_CTrainManagerTable[] = {

 // content:
       2,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   12, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors

 // slots: signature, parameters, type, tag, flags
      26,   20,   19,   19, 0x08,
      49,   43,   19,   19, 0x08,
      67,   19,   19,   19, 0x08,
      82,   19,   19,   19, 0x08,
      97,   19,   19,   19, 0x08,
     116,  111,   19,   19, 0x08,
     138,   19,   19,   19, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_CTrainManagerTable[] = {
    "CTrainManagerTable\0\0index\0setSimuMode(int)\0"
    "value\0setSimuSpeed(int)\0setSimuStart()\0"
    "setSimuPause()\0setSimuStop()\0mode\0"
    "setinterlockmode(int)\0setLanguage(int)\0"
};

const QMetaObject CTrainManagerTable::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_CTrainManagerTable,
      qt_meta_data_CTrainManagerTable, 0 }
};

const QMetaObject *CTrainManagerTable::metaObject() const
{
    return &staticMetaObject;
}

void *CTrainManagerTable::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CTrainManagerTable))
        return static_cast<void*>(const_cast< CTrainManagerTable*>(this));
    if (!strcmp(_clname, "Ui_CTrainManagerTable"))
        return static_cast< Ui_CTrainManagerTable*>(const_cast< CTrainManagerTable*>(this));
    return QDialog::qt_metacast(_clname);
}

int CTrainManagerTable::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: setSimuMode((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: setSimuSpeed((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: setSimuStart(); break;
        case 3: setSimuPause(); break;
        case 4: setSimuStop(); break;
        case 5: setinterlockmode((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 6: setLanguage((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 7;
    }
    return _id;
}
static const uint qt_meta_data_CLoginStudentTable[] = {

 // content:
       2,       // revision
       0,       // classname
       0,    0, // classinfo
      14,   12, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors

 // slots: signature, parameters, type, tag, flags
      22,   20,   19,   19, 0x0a,
      42,   19,   19,   19, 0x0a,
      61,   19,   19,   19, 0x0a,
      81,   19,   19,   19, 0x0a,
     101,   19,   19,   19, 0x0a,
     125,   19,   19,   19, 0x0a,
     139,   19,   19,   19, 0x0a,
     153,   19,   19,   19, 0x0a,
     167,   19,   19,   19, 0x0a,
     183,  181,   19,   19, 0x0a,
     201,   19,   19,   19, 0x0a,
     218,   19,   19,   19, 0x0a,
     236,   19,   19,   19, 0x0a,
     255,   19,   19,   19, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_CLoginStudentTable[] = {
    "CLoginStudentTable\0\0,\0selectCell(int,int)\0"
    "openSetTaskTable()\0openSetSceneTable()\0"
    "openSetFaultTable()\0openTrainManagerTable()\0"
    "setScene(int)\0setFault(int)\0logoutForce()\0"
    "updateModel()\0i\0settrainmode(int)\0"
    "logoutAllusers()\0setAllusersWait()\0"
    "setAllusersStart()\0setLanguage(int)\0"
};

const QMetaObject CLoginStudentTable::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_CLoginStudentTable,
      qt_meta_data_CLoginStudentTable, 0 }
};

const QMetaObject *CLoginStudentTable::metaObject() const
{
    return &staticMetaObject;
}

void *CLoginStudentTable::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CLoginStudentTable))
        return static_cast<void*>(const_cast< CLoginStudentTable*>(this));
    if (!strcmp(_clname, "Ui_CLoginStudentTable"))
        return static_cast< Ui_CLoginStudentTable*>(const_cast< CLoginStudentTable*>(this));
    return QDialog::qt_metacast(_clname);
}

int CLoginStudentTable::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: selectCell((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 1: openSetTaskTable(); break;
        case 2: openSetSceneTable(); break;
        case 3: openSetFaultTable(); break;
        case 4: openTrainManagerTable(); break;
        case 5: setScene((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 6: setFault((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 7: logoutForce(); break;
        case 8: updateModel(); break;
        case 9: settrainmode((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 10: logoutAllusers(); break;
        case 11: setAllusersWait(); break;
        case 12: setAllusersStart(); break;
        case 13: setLanguage((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 14;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
