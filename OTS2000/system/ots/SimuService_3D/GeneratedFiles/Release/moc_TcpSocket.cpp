/****************************************************************************
** Meta object code from reading C++ file 'TcpSocket.h'
**
** Created: Wed Apr 30 13:45:49 2014
**      by: The Qt Meta Object Compiler version 61 (Qt 4.5.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../TcpSocket.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'TcpSocket.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 61
#error "This file was generated using the moc from 4.5.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_Connection[] = {

 // content:
       2,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   12, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors

 // signals: signature, parameters, type, tag, flags
      36,   12,   11,   11, 0x05,
      85,   75,   11,   11, 0x05,
     126,  109,   11,   11, 0x05,

 // slots: signature, parameters, type, tag, flags
     148,   11,   11,   11, 0x0a,
     156,   75,   11,   11, 0x0a,
     180,   11,   11,   11, 0x0a,
     194,   11,   11,   11, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_Connection[] = {
    "Connection\0\0socketError,errorString\0"
    "error(QTcpSocket::SocketError,QString)\0"
    "byteArray\0recvMsgData(QByteArray)\0"
    "socketDescriptor\0deleteConnection(int)\0"
    "start()\0sendMsgData(QByteArray)\0"
    "recvMsgData()\0disconnected()\0"
};

const QMetaObject Connection::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_Connection,
      qt_meta_data_Connection, 0 }
};

const QMetaObject *Connection::metaObject() const
{
    return &staticMetaObject;
}

void *Connection::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Connection))
        return static_cast<void*>(const_cast< Connection*>(this));
    return QObject::qt_metacast(_clname);
}

int Connection::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: error((*reinterpret_cast< QTcpSocket::SocketError(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 1: recvMsgData((*reinterpret_cast< QByteArray(*)>(_a[1]))); break;
        case 2: deleteConnection((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: start(); break;
        case 4: sendMsgData((*reinterpret_cast< QByteArray(*)>(_a[1]))); break;
        case 5: recvMsgData(); break;
        case 6: disconnected(); break;
        default: ;
        }
        _id -= 7;
    }
    return _id;
}

// SIGNAL 0
void Connection::error(QTcpSocket::SocketError _t1, const QString & _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void Connection::recvMsgData(QByteArray _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void Connection::deleteConnection(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
static const uint qt_meta_data_TcpServer[] = {

 // content:
       2,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   12, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors

 // signals: signature, parameters, type, tag, flags
      21,   11,   10,   10, 0x05,

 // slots: signature, parameters, type, tag, flags
      45,   11,   10,   10, 0x08,
      93,   76,   10,   10, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_TcpServer[] = {
    "TcpServer\0\0byteArray\0sendMsgData(QByteArray)\0"
    "ProcessRecvMsgData(QByteArray)\0"
    "socketDescriptor\0deleteConnection(int)\0"
};

const QMetaObject TcpServer::staticMetaObject = {
    { &QTcpServer::staticMetaObject, qt_meta_stringdata_TcpServer,
      qt_meta_data_TcpServer, 0 }
};

const QMetaObject *TcpServer::metaObject() const
{
    return &staticMetaObject;
}

void *TcpServer::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_TcpServer))
        return static_cast<void*>(const_cast< TcpServer*>(this));
    return QTcpServer::qt_metacast(_clname);
}

int TcpServer::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QTcpServer::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: sendMsgData((*reinterpret_cast< QByteArray(*)>(_a[1]))); break;
        case 1: ProcessRecvMsgData((*reinterpret_cast< QByteArray(*)>(_a[1]))); break;
        case 2: deleteConnection((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 3;
    }
    return _id;
}

// SIGNAL 0
void TcpServer::sendMsgData(QByteArray _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
