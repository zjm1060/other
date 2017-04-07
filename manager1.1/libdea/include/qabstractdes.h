#ifndef QABSTRACTDES_H
#define QABSTRACTDES_H

#include "global.h"
#include "qidea.h"

typedef bool (*PSubKey)[16][48];

class QAbstractDES : public QIDEA
{
public:
	QAbstractDES();
	~QAbstractDES();

public:
	bool run(char *out,const char *in,long datalen,const char *key,int keylen,bool type = ENCRYPT);

protected:
	void des(char out[8], const char in[8], const PSubKey pSubKey, bool type);//标准DES加/解密
	void setKey(const char* key, int len);// 设置密钥
	void setSubKey(PSubKey pSubKey, const char key[8]);// 设置子密钥
	void fFunc(bool in[32], const bool ki[48]);// f 函数
	void sFunc(bool out[32], const bool in[48]);// S 盒代替
	void transform(bool *out, bool *in, const char *table, int len);// 变换
	void _xor(bool *inA, const bool *inB, int len);// 异或
	void rotateL(bool *in, int len, int loop);// 循环左移
	void byteToBit(bool *out, const char *in, int bits);// 字节组转换成位组
	void bitToByte(char *out, const bool *in, int bits);// 位组转换成字节组

private:
	bool m_subKey[2][16][48];// 16圈子密钥
	bool m_bIs3DES;// 3次DES标志
};

#endif // QABSTRACTDES_H
