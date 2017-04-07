/****************************************************************************
** Meta object code from reading C++ file 'contest.h'
**
** Created: Fri Jan 23 13:44:25 2015
**      by: The Qt Meta Object Compiler version 61 (Qt 4.5.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../src/contest.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'contest.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 61
#error "This file was generated using the moc from 4.5.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_CContestAddTable[] = {

 // content:
       2,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   12, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors

 // signals: signature, parameters, type, tag, flags
      18,   17,   17,   17, 0x05,
      33,   17,   17,   17, 0x05,

 // slots: signature, parameters, type, tag, flags
      48,   17,   17,   17, 0x08,
      56,   17,   17,   17, 0x08,
      64,   17,   17,   17, 0x0a,
      78,   17,   17,   17, 0x0a,
      93,   17,   17,   17, 0x0a,
     108,   17,   17,   17, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_CContestAddTable[] = {
    "CContestAddTable\0\0insertRecord()\0"
    "modifyRecord()\0apply()\0enter()\0"
    "displayTask()\0displayFault()\0"
    "displayScene()\0setLanguage(int)\0"
};

const QMetaObject CContestAddTable::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_CContestAddTable,
      qt_meta_data_CContestAddTable, 0 }
};

const QMetaObject *CContestAddTable::metaObject() const
{
    return &staticMetaObject;
}

void *CContestAddTable::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CContestAddTable))
        return static_cast<void*>(const_cast< CContestAddTable*>(this));
    if (!strcmp(_clname, "Ui_CContestAddTable"))
        return static_cast< Ui_CContestAddTable*>(const_cast< CContestAddTable*>(this));
    return QDialog::qt_metacast(_clname);
}

int CContestAddTable::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
        case 4: displayTask(); break;
        case 5: displayFault(); break;
        case 6: displayScene(); break;
        case 7: setLanguage((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 8;
    }
    return _id;
}

// SIGNAL 0
void CContestAddTable::insertRecord()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}

// SIGNAL 1
void CContestAddTable::modifyRecord()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}
static const uint qt_meta_data_CContestTable[] = {

 // content:
       2,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   12, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors

 // slots: signature, parameters, type, tag, flags
      19,   14,   15,   14, 0x0a,
      38,   36,   14,   14, 0x0a,
      58,   14,   14,   14, 0x0a,
      71,   14,   14,   14, 0x0a,
      88,   14,   14,   14, 0x0a,
     108,   14,   14,   14, 0x0a,
     121,   14,   14,   14, 0x0a,
     137,   14,   14,   14, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_CContestTable[] = {
    "CContestTable\0\0int\0displayContest()\0"
    ",\0selectCell(int,int)\0delContest()\0"
    "openContestAdd()\0openContestModify()\0"
    "addContest()\0modifyContest()\0"
    "setLanguage(int)\0"
};

const QMetaObject CContestTable::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_CContestTable,
      qt_meta_data_CContestTable, 0 }
};

const QMetaObject *CContestTable::metaObject() const
{
    return &staticMetaObject;
}

void *CContestTable::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CContestTable))
        return static_cast<void*>(const_cast< CContestTable*>(this));
    if (!strcmp(_clname, "Ui_CContestTable"))
        return static_cast< Ui_CContestTable*>(const_cast< CContestTable*>(this));
    return QDialog::qt_metacast(_clname);
}

int CContestTable::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: { int _r = displayContest();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 1: selectCell((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 2: delContest(); break;
        case 3: openContestAdd(); break;
        case 4: openContestModify(); break;
        case 5: addContest(); break;
        case 6: modifyContest(); break;
        case 7: setLanguage((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 8;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
