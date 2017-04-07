/****************************************************************************
** Meta object code from reading C++ file 'task.h'
**
** Created: Fri Jan 23 13:43:36 2015
**      by: The Qt Meta Object Compiler version 61 (Qt 4.5.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../src/task.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'task.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 61
#error "This file was generated using the moc from 4.5.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_CTaskAddTable[] = {

 // content:
       2,       // revision
       0,       // classname
       0,    0, // classinfo
      19,   12, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors

 // signals: signature, parameters, type, tag, flags
      15,   14,   14,   14, 0x05,
      30,   14,   14,   14, 0x05,

 // slots: signature, parameters, type, tag, flags
      45,   14,   14,   14, 0x08,
      53,   14,   14,   14, 0x08,
      61,   14,   14,   14, 0x08,
      82,   78,   14,   14, 0x08,
      98,   14,   14,   14, 0x08,
     119,   14,   14,   14, 0x08,
     134,   14,   14,   14, 0x08,
     151,   14,   14,   14, 0x08,
     168,   14,   14,   14, 0x08,
     188,   14,   14,   14, 0x08,
     201,   14,   14,   14, 0x08,
     217,   14,   14,   14, 0x08,
     233,   14,   14,   14, 0x08,
     242,   14,   14,   14, 0x08,
     253,   14,   14,   14, 0x08,
     268,   14,   14,   14, 0x0a,
     283,   14,   14,   14, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_CTaskAddTable[] = {
    "CTaskAddTable\0\0insertRecord()\0"
    "modifyRecord()\0apply()\0enter()\0"
    "displayStation()\0stn\0displayDev(int)\0"
    "displayTaskGrpName()\0displayScene()\0"
    "displaySubject()\0openAddSubject()\0"
    "openModifySubject()\0delSubject()\0"
    "importSubject()\0exportSubject()\0"
    "moveUp()\0moveDown()\0setScoreAuto()\0"
    "displayFault()\0setLanguage(int)\0"
};

const QMetaObject CTaskAddTable::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_CTaskAddTable,
      qt_meta_data_CTaskAddTable, 0 }
};

const QMetaObject *CTaskAddTable::metaObject() const
{
    return &staticMetaObject;
}

void *CTaskAddTable::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CTaskAddTable))
        return static_cast<void*>(const_cast< CTaskAddTable*>(this));
    if (!strcmp(_clname, "Ui_CTaskAddTable"))
        return static_cast< Ui_CTaskAddTable*>(const_cast< CTaskAddTable*>(this));
    return QDialog::qt_metacast(_clname);
}

int CTaskAddTable::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
        case 4: displayStation(); break;
        case 5: displayDev((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 6: displayTaskGrpName(); break;
        case 7: displayScene(); break;
        case 8: displaySubject(); break;
        case 9: openAddSubject(); break;
        case 10: openModifySubject(); break;
        case 11: delSubject(); break;
        case 12: importSubject(); break;
        case 13: exportSubject(); break;
        case 14: moveUp(); break;
        case 15: moveDown(); break;
        case 16: setScoreAuto(); break;
        case 17: displayFault(); break;
        case 18: setLanguage((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 19;
    }
    return _id;
}

// SIGNAL 0
void CTaskAddTable::insertRecord()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}

// SIGNAL 1
void CTaskAddTable::modifyRecord()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}
static const uint qt_meta_data_CTaskTable[] = {

 // content:
       2,       // revision
       0,       // classname
       0,    0, // classinfo
      14,   12, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors

 // slots: signature, parameters, type, tag, flags
      16,   11,   12,   11, 0x0a,
      32,   30,   11,   11, 0x0a,
      52,   11,   11,   11, 0x0a,
      62,   11,   11,   11, 0x0a,
      76,   11,   11,   11, 0x0a,
      93,   11,   11,   11, 0x0a,
     108,   11,   11,   11, 0x0a,
     118,   11,   11,   11, 0x0a,
     131,   11,   11,   11, 0x0a,
     144,   11,   11,   11, 0x0a,
     163,   30,   11,   11, 0x0a,
     186,   11,   11,   11, 0x0a,
     201,  199,   11,   11, 0x0a,
     224,   11,   11,   11, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_CTaskTable[] = {
    "CTaskTable\0\0int\0displayTask()\0,\0"
    "selectCell(int,int)\0delTask()\0"
    "openTaskAdd()\0openTaskModify()\0"
    "openTaskCopy()\0addTask()\0openStnDev()\0"
    "modifyTask()\0displayUpdate(int)\0"
    "displayUpdate(int,int)\0selectTask()\0"
    "i\0displaytaskselect(int)\0setLanguage(int)\0"
};

const QMetaObject CTaskTable::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_CTaskTable,
      qt_meta_data_CTaskTable, 0 }
};

const QMetaObject *CTaskTable::metaObject() const
{
    return &staticMetaObject;
}

void *CTaskTable::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CTaskTable))
        return static_cast<void*>(const_cast< CTaskTable*>(this));
    if (!strcmp(_clname, "Ui_CTaskTable"))
        return static_cast< Ui_CTaskTable*>(const_cast< CTaskTable*>(this));
    return QDialog::qt_metacast(_clname);
}

int CTaskTable::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: { int _r = displayTask();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 1: selectCell((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 2: delTask(); break;
        case 3: openTaskAdd(); break;
        case 4: openTaskModify(); break;
        case 5: openTaskCopy(); break;
        case 6: addTask(); break;
        case 7: openStnDev(); break;
        case 8: modifyTask(); break;
        case 9: displayUpdate((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 10: displayUpdate((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 11: selectTask(); break;
        case 12: displaytaskselect((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 13: setLanguage((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 14;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
