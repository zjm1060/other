/****************************************************************************
** Meta object code from reading C++ file 'scene.h'
**
** Created: Fri Jan 23 13:44:02 2015
**      by: The Qt Meta Object Compiler version 61 (Qt 4.5.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../src/scene.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'scene.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 61
#error "This file was generated using the moc from 4.5.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_CSceneAddTable[] = {

 // content:
       2,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   12, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors

 // signals: signature, parameters, type, tag, flags
      16,   15,   15,   15, 0x05,
      31,   15,   15,   15, 0x05,

 // slots: signature, parameters, type, tag, flags
      46,   15,   15,   15, 0x0a,
      63,   15,   15,   15, 0x08,
      71,   15,   15,   15, 0x08,
      79,   15,   15,   15, 0x08,
     100,   96,   15,   15, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_CSceneAddTable[] = {
    "CSceneAddTable\0\0insertRecord()\0"
    "modifyRecord()\0setLanguage(int)\0apply()\0"
    "enter()\0displayStation()\0stn\0"
    "displayDev(int)\0"
};

const QMetaObject CSceneAddTable::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_CSceneAddTable,
      qt_meta_data_CSceneAddTable, 0 }
};

const QMetaObject *CSceneAddTable::metaObject() const
{
    return &staticMetaObject;
}

void *CSceneAddTable::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CSceneAddTable))
        return static_cast<void*>(const_cast< CSceneAddTable*>(this));
    if (!strcmp(_clname, "Ui_CSceneAddTable"))
        return static_cast< Ui_CSceneAddTable*>(const_cast< CSceneAddTable*>(this));
    return QDialog::qt_metacast(_clname);
}

int CSceneAddTable::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
void CSceneAddTable::insertRecord()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}

// SIGNAL 1
void CSceneAddTable::modifyRecord()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}
static const uint qt_meta_data_CSceneTable[] = {

 // content:
       2,       // revision
       0,       // classname
       0,    0, // classinfo
      21,   12, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors

 // slots: signature, parameters, type, tag, flags
      13,   12,   12,   12, 0x08,
      35,   12,   12,   12, 0x08,
      67,   12,   63,   12, 0x0a,
      84,   82,   12,   12, 0x0a,
     104,   12,   12,   12, 0x0a,
     115,   12,   12,   12, 0x0a,
     131,   12,   12,   12, 0x0a,
     146,   12,   12,   12, 0x0a,
     164,   12,   12,   12, 0x0a,
     177,   12,   12,   12, 0x0a,
     188,   12,   12,   12, 0x0a,
     202,   12,   12,   12, 0x0a,
     214,   12,   12,   12, 0x0a,
     226,   12,   12,   12, 0x0a,
     245,   82,   12,   12, 0x0a,
     268,   12,   12,   12, 0x0a,
     291,   82,   12,   12, 0x0a,
     321,   12,   12,   12, 0x0a,
     341,   12,   12,   12, 0x0a,
     355,   12,   12,   12, 0x0a,
     372,   12,   12,   12, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_CSceneTable[] = {
    "CSceneTable\0\0menuListScene(QPoint)\0"
    "menuListSceneMerger(QPoint)\0int\0"
    "displayScene()\0,\0selectCell(int,int)\0"
    "delScene()\0openSceneData()\0openSceneAdd()\0"
    "openSceneModify()\0openStnDev()\0"
    "addScene()\0modifyScene()\0intoScene()\0"
    "copyScene()\0displayUpdate(int)\0"
    "displayUpdate(int,int)\0addSceneToMergerList()\0"
    "addSceneToMergerList(int,int)\0"
    "delFromMergerList()\0clearMerger()\0"
    "runSceneMerger()\0setLanguage(int)\0"
};

const QMetaObject CSceneTable::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_CSceneTable,
      qt_meta_data_CSceneTable, 0 }
};

const QMetaObject *CSceneTable::metaObject() const
{
    return &staticMetaObject;
}

void *CSceneTable::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CSceneTable))
        return static_cast<void*>(const_cast< CSceneTable*>(this));
    if (!strcmp(_clname, "Ui_CSceneTable"))
        return static_cast< Ui_CSceneTable*>(const_cast< CSceneTable*>(this));
    return QDialog::qt_metacast(_clname);
}

int CSceneTable::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: menuListScene((*reinterpret_cast< const QPoint(*)>(_a[1]))); break;
        case 1: menuListSceneMerger((*reinterpret_cast< const QPoint(*)>(_a[1]))); break;
        case 2: { int _r = displayScene();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 3: selectCell((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 4: delScene(); break;
        case 5: openSceneData(); break;
        case 6: openSceneAdd(); break;
        case 7: openSceneModify(); break;
        case 8: openStnDev(); break;
        case 9: addScene(); break;
        case 10: modifyScene(); break;
        case 11: intoScene(); break;
        case 12: copyScene(); break;
        case 13: displayUpdate((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 14: displayUpdate((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 15: addSceneToMergerList(); break;
        case 16: addSceneToMergerList((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 17: delFromMergerList(); break;
        case 18: clearMerger(); break;
        case 19: runSceneMerger(); break;
        case 20: setLanguage((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 21;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
