/****************************************************************************
** Meta object code from reading C++ file 'Sequence.h'
**
** Created: Thu Jan 15 10:27:03 2015
**      by: The Qt Meta Object Compiler version 61 (Qt 4.5.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../src/Sequence.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'Sequence.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 61
#error "This file was generated using the moc from 4.5.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_SequenceList[] = {

 // content:
       2,       // revision
       0,       // classname
       0,    0, // classinfo
      14,   12, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors

 // slots: signature, parameters, type, tag, flags
      14,   13,   13,   13, 0x08,
      25,   13,   13,   13, 0x08,
      36,   13,   13,   13, 0x08,
      50,   13,   13,   13, 0x08,
      64,   13,   13,   13, 0x08,
      76,   13,   13,   13, 0x08,
      90,   13,   13,   13, 0x08,
     109,  105,   13,   13, 0x08,
     129,   13,   13,   13, 0x08,
     150,   13,   13,   13, 0x08,
     165,   13,   13,   13, 0x08,
     178,   13,   13,   13, 0x08,
     199,  197,   13,   13, 0x08,
     222,   13,   13,   13, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_SequenceList[] = {
    "SequenceList\0\0onBtnAdd()\0onBtnDel()\0"
    "onBtnModify()\0onBtnCancel()\0onRefresh()\0"
    "addSequence()\0modiSequence()\0i,j\0"
    "selectCell(int,int)\0openSequenceConfig()\0"
    "intoSequence()\0openStnDev()\0"
    "displayUpdate(int)\0,\0displayUpdate(int,int)\0"
    "setLanguage(int)\0"
};

const QMetaObject SequenceList::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_SequenceList,
      qt_meta_data_SequenceList, 0 }
};

const QMetaObject *SequenceList::metaObject() const
{
    return &staticMetaObject;
}

void *SequenceList::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_SequenceList))
        return static_cast<void*>(const_cast< SequenceList*>(this));
    if (!strcmp(_clname, "Ui_SequenceList"))
        return static_cast< Ui_SequenceList*>(const_cast< SequenceList*>(this));
    return QDialog::qt_metacast(_clname);
}

int SequenceList::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: onBtnAdd(); break;
        case 1: onBtnDel(); break;
        case 2: onBtnModify(); break;
        case 3: onBtnCancel(); break;
        case 4: onRefresh(); break;
        case 5: addSequence(); break;
        case 6: modiSequence(); break;
        case 7: selectCell((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 8: openSequenceConfig(); break;
        case 9: intoSequence(); break;
        case 10: openStnDev(); break;
        case 11: displayUpdate((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 12: displayUpdate((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 13: setLanguage((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 14;
    }
    return _id;
}
static const uint qt_meta_data_SequenceEdit[] = {

 // content:
       2,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   12, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors

 // signals: signature, parameters, type, tag, flags
      14,   13,   13,   13, 0x05,
      29,   13,   13,   13, 0x05,

 // slots: signature, parameters, type, tag, flags
      44,   13,   13,   13, 0x0a,
      61,   13,   13,   13, 0x08,
      71,   13,   13,   13, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_SequenceEdit[] = {
    "SequenceEdit\0\0insertRecord()\0"
    "modifyRecord()\0setLanguage(int)\0"
    "onBtnOK()\0onBtnCancel()\0"
};

const QMetaObject SequenceEdit::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_SequenceEdit,
      qt_meta_data_SequenceEdit, 0 }
};

const QMetaObject *SequenceEdit::metaObject() const
{
    return &staticMetaObject;
}

void *SequenceEdit::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_SequenceEdit))
        return static_cast<void*>(const_cast< SequenceEdit*>(this));
    if (!strcmp(_clname, "Ui_SequenceEdit"))
        return static_cast< Ui_SequenceEdit*>(const_cast< SequenceEdit*>(this));
    return QDialog::qt_metacast(_clname);
}

int SequenceEdit::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: insertRecord(); break;
        case 1: modifyRecord(); break;
        case 2: setLanguage((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: onBtnOK(); break;
        case 4: onBtnCancel(); break;
        default: ;
        }
        _id -= 5;
    }
    return _id;
}

// SIGNAL 0
void SequenceEdit::insertRecord()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}

// SIGNAL 1
void SequenceEdit::modifyRecord()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}
QT_END_MOC_NAMESPACE
