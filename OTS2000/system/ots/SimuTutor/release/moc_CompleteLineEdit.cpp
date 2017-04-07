/****************************************************************************
** Meta object code from reading C++ file 'CompleteLineEdit.h'
**
** Created: Thu Jan 15 10:28:18 2015
**      by: The Qt Meta Object Compiler version 61 (Qt 4.5.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../src/CompleteLineEdit.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'CompleteLineEdit.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 61
#error "This file was generated using the moc from 4.5.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_CompleteLineEdit[] = {

 // content:
       2,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   12, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors

 // signals: signature, parameters, type, tag, flags
      23,   18,   17,   17, 0x05,

 // slots: signature, parameters, type, tag, flags
      58,   53,   17,   17, 0x09,
      93,   82,   17,   17, 0x09,

       0        // eod
};

static const char qt_meta_stringdata_CompleteLineEdit[] = {
    "CompleteLineEdit\0\0item\0"
    "itemClicked(QListWidgetItem*)\0text\0"
    "textEditedSlot(QString)\0modelIndex\0"
    "clickedSlot(QModelIndex)\0"
};

const QMetaObject CompleteLineEdit::staticMetaObject = {
    { &QLineEdit::staticMetaObject, qt_meta_stringdata_CompleteLineEdit,
      qt_meta_data_CompleteLineEdit, 0 }
};

const QMetaObject *CompleteLineEdit::metaObject() const
{
    return &staticMetaObject;
}

void *CompleteLineEdit::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CompleteLineEdit))
        return static_cast<void*>(const_cast< CompleteLineEdit*>(this));
    return QLineEdit::qt_metacast(_clname);
}

int CompleteLineEdit::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QLineEdit::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: itemClicked((*reinterpret_cast< QListWidgetItem*(*)>(_a[1]))); break;
        case 1: textEditedSlot((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 2: clickedSlot((*reinterpret_cast< QModelIndex(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 3;
    }
    return _id;
}

// SIGNAL 0
void CompleteLineEdit::itemClicked(QListWidgetItem * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
