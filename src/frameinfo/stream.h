#ifndef Stream_h_1454
#define Stream_h_1454


class Stream
{
protected:
	char* data;
	unsigned int len;
	unsigned int position;

public:

	Stream():data(0),len(0), position(0) {}
	explicit Stream(int size):data(new char[size]),len(size), position(0) {}
	Stream(int size, const void*src):data(new char[size]),len(size), position(0) 
	{
		write(src,size);
		position = 0;
	}

	~Stream()
	{
		delete[] data;
	}

	Stream* subStream(unsigned int shift, unsigned int size)
	{
		if (shift+size > len)
			return NULL;


		Stream* str = new Stream(size);
		str->write(data+shift,size);
		str->setPosition(0);
		return str;
	}

	Stream& operator=(const Stream& right)
	{
		delete[] data;

		len = right.len;
		position = right.position;
		data = new char[len];
		memcpy(data,right.data,len);
		return *this;
	}

	int read(void* dst, unsigned int size)
	{
		int haveToread = position+size < len ? size : len - position;
		memcpy(dst, data + position , haveToread);
		position += haveToread;
		return haveToread;
	}

	int write(const void* src, unsigned int size)
	{
		int haveToWrite = position+size < len ? size : len - position;
		memcpy(data + position, src, haveToWrite);
		position += haveToWrite;
		return haveToWrite;
	}


	int getSize()const {return len;} 
	int getPosition()const {return position;} 
	void setPosition(unsigned int pos) {position = pos > len ? len : pos;}
	int available() const {return len - position;}
	const char* getData(){return data+position;};


	Stream(const Stream& r)
	{
		(*this) = r;
	}

private:
	Stream& operator=(Stream& right){};

};

#endif //Stream_h_1454