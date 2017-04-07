/****************************************************************************
** Meta object code from reading C++ file 'OfficeContainer.h'
**
** Created: Wed Jan 14 10:22:44 2015
**      by: The Qt Meta Object Compiler version 61 (Qt 4.5.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../src/OfficeContainer.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'OfficeContainer.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 61
#error "This file was generated using the moc from 4.5.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_OfficeContainer[] = {

 // content:
       2,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   12, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors

 // slots: signature, parameters, type, tag, flags
      17,   16,   16,   16, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_OfficeContainer[] = {
    "OfficeContainer\0\0close_wait()\0"
};

const QMetaObject OfficeContainer::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_OfficeContainer,
      qt_meta_data_OfficeContainer, 0 }
};

const QMetaObject *OfficeContainer::metaObject() const
{
    return &staticMetaObject;
}

void *OfficeContainer::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_OfficeContainer))
        return static_cast<void*>(const_cast< OfficeContainer*>(this));
    return QWidget::qt_metacast(_clname);
}

int OfficeContainer::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: close_wait(); break;
        default: ;
        }
        _id -= 1;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
