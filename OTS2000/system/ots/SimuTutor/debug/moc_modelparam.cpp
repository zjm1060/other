/****************************************************************************
** Meta object code from reading C++ file 'modelparam.h'
**
** Created: Fri Jan 23 13:44:10 2015
**      by: The Qt Meta Object Compiler version 61 (Qt 4.5.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../src/modelparam.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'modelparam.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 61
#error "This file was generated using the moc from 4.5.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_CModelParamAddForm[] = {

 // content:
       2,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   12, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors

 // signals: signature, parameters, type, tag, flags
      20,   19,   19,   19, 0x05,
      35,   19,   19,   19, 0x05,

 // slots: signature, parameters, type, tag, flags
      50,   19,   19,   19, 0x08,
      58,   19,   19,   19, 0x08,
      66,   19,   19,   19, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_CModelParamAddForm[] = {
    "CModelParamAddForm\0\0insertRecord()\0"
    "modifyRecord()\0apply()\0enter()\0"
    "setLanguage(int)\0"
};

const QMetaObject CModelParamAddForm::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_CModelParamAddForm,
      qt_meta_data_CModelParamAddForm, 0 }
};

const QMetaObject *CModelParamAddForm::metaObject() const
{
    return &staticMetaObject;
}

void *CModelParamAddForm::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CModelParamAddForm))
        return static_cast<void*>(const_cast< CModelParamAddForm*>(this));
    if (!strcmp(_clname, "Ui_CModelParamAddForm"))
        return static_cast< Ui_CModelParamAddForm*>(const_cast< CModelParamAddForm*>(this));
    return QDialog::qt_metacast(_clname);
}

int CModelParamAddForm::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
void CModelParamAddForm::insertRecord()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}

// SIGNAL 1
void CModelParamAddForm::modifyRecord()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}
static const uint qt_meta_data_CModelParamForm[] = {

 // content:
       2,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   12, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors

 // slots: signature, parameters, type, tag, flags
      17,   16,   16,   16, 0x0a,
      37,   16,   16,   16, 0x0a,
      60,   16,   16,   16, 0x0a,
      78,   76,   16,   16, 0x0a,
      98,   16,   16,   16, 0x0a,
     114,   16,   16,   16, 0x0a,
     137,   16,  133,   16, 0x0a,
     157,   16,   16,   16, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_CModelParamForm[] = {
    "CModelParamForm\0\0openModelParamAdd()\0"
    "openModelParamModify()\0addModelParam()\0"
    ",\0selectCell(int,int)\0delModelParam()\0"
    "modifyModelParam()\0int\0displayModelParam()\0"
    "setLanguage(int)\0"
};

const QMetaObject CModelParamForm::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_CModelParamForm,
      qt_meta_data_CModelParamForm, 0 }
};

const QMetaObject *CModelParamForm::metaObject() const
{
    return &staticMetaObject;
}

void *CModelParamForm::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CModelParamForm))
        return static_cast<void*>(const_cast< CModelParamForm*>(this));
    if (!strcmp(_clname, "Ui_CModelParamForm"))
        return static_cast< Ui_CModelParamForm*>(const_cast< CModelParamForm*>(this));
    return QDialog::qt_metacast(_clname);
}

int CModelParamForm::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: openModelParamAdd(); break;
        case 1: openModelParamModify(); break;
        case 2: addModelParam(); break;
        case 3: selectCell((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 4: delModelParam(); break;
        case 5: modifyModelParam(); break;
        case 6: { int _r = displayModelParam();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 7: setLanguage((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 8;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
