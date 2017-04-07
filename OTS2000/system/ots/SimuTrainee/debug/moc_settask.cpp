/****************************************************************************
** Meta object code from reading C++ file 'settask.h'
**
** Created: Thu Jan 22 17:27:30 2015
**      by: The Qt Meta Object Compiler version 61 (Qt 4.5.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../SimuTutor/src/settask.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'settask.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 61
#error "This file was generated using the moc from 4.5.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_CSetTaskTable[] = {

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

 // slots: signature, parameters, type, tag, flags
      32,   14,   14,   14, 0x0a,
      47,   14,   14,   14, 0x0a,
      63,   14,   14,   14, 0x0a,
      79,   14,   14,   14, 0x0a,
      89,   14,   14,   14, 0x0a,
     106,   14,   14,   14, 0x0a,
     122,   14,   14,   14, 0x0a,
     133,   14,   14,   14, 0x0a,
     146,   14,   14,   14, 0x0a,
     168,  162,   14,   14, 0x0a,
     195,  162,   14,   14, 0x0a,
     223,  162,   14,   14, 0x0a,
     251,   14,   14,   14, 0x0a,
     267,   14,   14,   14, 0x0a,
     284,   14,   14,   14, 0x0a,
     309,  301,   14,   14, 0x0a,
     333,  301,   14,   14, 0x0a,
     351,   14,   14,   14, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_CSetTaskTable[] = {
    "CSetTaskTable\0\0sig_tasksetted()\0"
    "setTaskModel()\0setSceneModel()\0"
    "setFaultModel()\0setTask()\0singleMode(bool)\0"
    "multiMode(bool)\0sendTask()\0displayStn()\0"
    "displayDev(int)\0index\0taskFilterTypeChanged(int)\0"
    "sceneFilterTypeChanged(int)\0"
    "faultFilterTypeChanged(int)\0setTaskFilter()\0"
    "setSceneFilter()\0setFaultFilter()\0"
    "checked\0setSortbyusertype(bool)\0"
    "setExamOnly(bool)\0setLanguage(int)\0"
};

const QMetaObject CSetTaskTable::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_CSetTaskTable,
      qt_meta_data_CSetTaskTable, 0 }
};

const QMetaObject *CSetTaskTable::metaObject() const
{
    return &staticMetaObject;
}

void *CSetTaskTable::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CSetTaskTable))
        return static_cast<void*>(const_cast< CSetTaskTable*>(this));
    if (!strcmp(_clname, "Ui_CSetTaskTable"))
        return static_cast< Ui_CSetTaskTable*>(const_cast< CSetTaskTable*>(this));
    return QWidget::qt_metacast(_clname);
}

int CSetTaskTable::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: sig_tasksetted(); break;
        case 1: setTaskModel(); break;
        case 2: setSceneModel(); break;
        case 3: setFaultModel(); break;
        case 4: setTask(); break;
        case 5: singleMode((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 6: multiMode((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 7: sendTask(); break;
        case 8: displayStn(); break;
        case 9: displayDev((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 10: taskFilterTypeChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 11: sceneFilterTypeChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 12: faultFilterTypeChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 13: setTaskFilter(); break;
        case 14: setSceneFilter(); break;
        case 15: setFaultFilter(); break;
        case 16: setSortbyusertype((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 17: setExamOnly((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 18: setLanguage((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 19;
    }
    return _id;
}

// SIGNAL 0
void CSetTaskTable::sig_tasksetted()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}
static const uint qt_meta_data_CSetSceneFaultTable[] = {

 // content:
       2,       // revision
       0,       // classname
       0,    0, // classinfo
       9,   12, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors

 // signals: signature, parameters, type, tag, flags
      21,   20,   20,   20, 0x05,
      39,   20,   20,   20, 0x05,
      57,   20,   20,   20, 0x05,

 // slots: signature, parameters, type, tag, flags
      68,   20,   20,   20, 0x08,
      82,   20,   20,   20, 0x08,
      93,   20,   20,   20, 0x08,
     104,   20,   20,   20, 0x08,
     116,   20,   20,   20, 0x08,
     126,   20,   20,   20, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_CSetSceneFaultTable[] = {
    "CSetSceneFaultTable\0\0sigSendScene(int)\0"
    "sigSendFault(int)\0closedlg()\0refreshList()\0"
    "setScene()\0setFault()\0setFilter()\0"
    "onclose()\0setLanguage(int)\0"
};

const QMetaObject CSetSceneFaultTable::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_CSetSceneFaultTable,
      qt_meta_data_CSetSceneFaultTable, 0 }
};

const QMetaObject *CSetSceneFaultTable::metaObject() const
{
    return &staticMetaObject;
}

void *CSetSceneFaultTable::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CSetSceneFaultTable))
        return static_cast<void*>(const_cast< CSetSceneFaultTable*>(this));
    if (!strcmp(_clname, "Ui_CSetSceneFaultTable"))
        return static_cast< Ui_CSetSceneFaultTable*>(const_cast< CSetSceneFaultTable*>(this));
    return QWidget::qt_metacast(_clname);
}

int CSetSceneFaultTable::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: sigSendScene((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: sigSendFault((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: closedlg(); break;
        case 3: refreshList(); break;
        case 4: setScene(); break;
        case 5: setFault(); break;
        case 6: setFilter(); break;
        case 7: onclose(); break;
        case 8: setLanguage((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 9;
    }
    return _id;
}

// SIGNAL 0
void CSetSceneFaultTable::sigSendScene(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void CSetSceneFaultTable::sigSendFault(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void CSetSceneFaultTable::closedlg()
{
    QMetaObject::activate(this, &staticMetaObject, 2, 0);
}
QT_END_MOC_NAMESPACE
