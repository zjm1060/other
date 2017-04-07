/****************************************************************************
** Meta object code from reading C++ file 'sendmessage.h'
**
** Created: Fri Jan 23 13:43:57 2015
**      by: The Qt Meta Object Compiler version 61 (Qt 4.5.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../src/sendmessage.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'sendmessage.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 61
#error "This file was generated using the moc from 4.5.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_CharacterWidget[] = {

 // content:
       2,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   12, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors

 // signals: signature, parameters, type, tag, flags
      27,   17,   16,   16, 0x05,

       0        // eod
};

static const char qt_meta_stringdata_CharacterWidget[] = {
    "CharacterWidget\0\0character\0"
    "characterSelected(QString)\0"
};

const QMetaObject CharacterWidget::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_CharacterWidget,
      qt_meta_data_CharacterWidget, 0 }
};

const QMetaObject *CharacterWidget::metaObject() const
{
    return &staticMetaObject;
}

void *CharacterWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CharacterWidget))
        return static_cast<void*>(const_cast< CharacterWidget*>(this));
    return QWidget::qt_metacast(_clname);
}

int CharacterWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: characterSelected((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 1;
    }
    return _id;
}

// SIGNAL 0
void CharacterWidget::characterSelected(const QString & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
static const uint qt_meta_data_CFaceImageWidget[] = {

 // content:
       2,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   12, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors

 // signals: signature, parameters, type, tag, flags
      18,   17,   17,   17, 0x05,

       0        // eod
};

static const char qt_meta_stringdata_CFaceImageWidget[] = {
    "CFaceImageWidget\0\0sigToggled(bool)\0"
};

const QMetaObject CFaceImageWidget::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_CFaceImageWidget,
      qt_meta_data_CFaceImageWidget, 0 }
};

const QMetaObject *CFaceImageWidget::metaObject() const
{
    return &staticMetaObject;
}

void *CFaceImageWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CFaceImageWidget))
        return static_cast<void*>(const_cast< CFaceImageWidget*>(this));
    return QWidget::qt_metacast(_clname);
}

int CFaceImageWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: sigToggled((*reinterpret_cast< bool(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 1;
    }
    return _id;
}

// SIGNAL 0
void CFaceImageWidget::sigToggled(bool _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
static const uint qt_meta_data_CSendMessageTable[] = {

 // content:
       2,       // revision
       0,       // classname
       0,    0, // classinfo
      22,   12, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors

 // signals: signature, parameters, type, tag, flags
      19,   18,   18,   18, 0x05,

 // slots: signature, parameters, type, tag, flags
      32,   18,   18,   18, 0x08,
      52,   18,   18,   18, 0x08,
      75,   18,   18,   18, 0x08,
      89,   18,   18,   18, 0x08,
     105,  101,   18,   18, 0x08,
     128,  101,   18,   18, 0x08,
     152,   18,   18,   18, 0x08,
     169,   18,   18,   18, 0x08,
     185,   18,   18,   18, 0x08,
     212,   18,   18,   18, 0x08,
     231,   18,   18,   18, 0x08,
     247,   18,   18,   18, 0x08,
     274,   18,   18,   18, 0x08,
     293,   18,   18,   18, 0x08,
     316,   18,   18,   18, 0x08,
     330,   18,   18,   18, 0x08,
     341,   18,   18,   18, 0x08,
     365,  351,   18,   18, 0x08,
     393,   18,   18,   18, 0x08,
     425,  419,   18,   18, 0x08,
     450,   18,   18,   18, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_CSendMessageTable[] = {
    "CSendMessageTable\0\0sigRecvMsg()\0"
    "changeAllUserShow()\0changeMsgRecShow(bool)\0"
    "sendMessage()\0sendShake()\0msg\0"
    "appendMsgRec(OTS_MSG*)\0appendMsgShow(OTS_MSG*)\0"
    "displayAllUser()\0addToSendUser()\0"
    "addToSendUser(QModelIndex)\0"
    "addAllToSendUser()\0delToSendUser()\0"
    "delToSendUser(QModelIndex)\0"
    "delAllToSendUser()\0readPendingDatagrams()\0"
    "insertImage()\0setColor()\0setFont()\0"
    "user_id,state\0userStateChanged(int,short)\0"
    "openFaceImageWidget(bool)\0index\0"
    "on_usefulExpression(int)\0setLanguage(int)\0"
};

const QMetaObject CSendMessageTable::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_CSendMessageTable,
      qt_meta_data_CSendMessageTable, 0 }
};

const QMetaObject *CSendMessageTable::metaObject() const
{
    return &staticMetaObject;
}

void *CSendMessageTable::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CSendMessageTable))
        return static_cast<void*>(const_cast< CSendMessageTable*>(this));
    if (!strcmp(_clname, "Ui_CSendMessageTable"))
        return static_cast< Ui_CSendMessageTable*>(const_cast< CSendMessageTable*>(this));
    return QWidget::qt_metacast(_clname);
}

int CSendMessageTable::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: sigRecvMsg(); break;
        case 1: changeAllUserShow(); break;
        case 2: changeMsgRecShow((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 3: sendMessage(); break;
        case 4: sendShake(); break;
        case 5: appendMsgRec((*reinterpret_cast< OTS_MSG*(*)>(_a[1]))); break;
        case 6: appendMsgShow((*reinterpret_cast< OTS_MSG*(*)>(_a[1]))); break;
        case 7: displayAllUser(); break;
        case 8: addToSendUser(); break;
        case 9: addToSendUser((*reinterpret_cast< const QModelIndex(*)>(_a[1]))); break;
        case 10: addAllToSendUser(); break;
        case 11: delToSendUser(); break;
        case 12: delToSendUser((*reinterpret_cast< const QModelIndex(*)>(_a[1]))); break;
        case 13: delAllToSendUser(); break;
        case 14: readPendingDatagrams(); break;
        case 15: insertImage(); break;
        case 16: setColor(); break;
        case 17: setFont(); break;
        case 18: userStateChanged((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< short(*)>(_a[2]))); break;
        case 19: openFaceImageWidget((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 20: on_usefulExpression((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 21: setLanguage((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 22;
    }
    return _id;
}

// SIGNAL 0
void CSendMessageTable::sigRecvMsg()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}
QT_END_MOC_NAMESPACE
