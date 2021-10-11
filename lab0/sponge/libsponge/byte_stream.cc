#include "byte_stream.hh"

// Dummy implementation of a flow-controlled in-memory byte stream.

// For Lab 0, please replace with a real implementation that passes the
// automated checks run by `make check_lab0`.

// You will need to add private members to the class declaration in `byte_stream.hh`

template <typename... Targs>
void DUMMY_CODE(Targs &&... /* unused */) {}

using namespace std;

ByteStream::ByteStream(const size_t capacity){
    this->_capacity = capacity;
    _buffer=string(capacity+1,0);
    rpoint=0;
    wpoint=0;
    _end_input=false;
    freesize=capacity;
}

size_t ByteStream::write(const string &data) {
    if (input_ended()){
        return 0;
    }

    if(remaining_capacity()==0){
        return 0;
    }
    const size_t size = min(data.size(),freesize);

    for(int i=0;i<static_cast<int>(size);i++){
        _buffer[wpoint]=data[i];
        wpoint = (wpoint+1)%_capacity;
    }
    freesize-=size;
    bytes_write_all+=size;
    return size;
}

//! \param[in] len bytes will be copied from the output side of the buffer
string ByteStream::peek_output(const size_t len)const {
    size_t size = min(len,_capacity-freesize);
    string res(size,0);
    size_t rp = this->rpoint;
    for (int i=0;i<static_cast<int>(size);i++){
        res[i] = _buffer[rp];
        rp = (rp+1)%_capacity;
    }
    return res;
}

//! \param[in] len bytes will be removed from the output side of the buffer
void ByteStream::pop_output(const size_t len) {
    size_t size = min(len,_capacity-freesize);
    rpoint = (rpoint+size)%_capacity;
    freesize+=size;
    bytes_read_all+= size;
}

//! Read (i.e., copy and then pop) the next "len" bytes of the stream
//! \param[in] len bytes will be popped and returned
//! \returns a string
std::string ByteStream::read(const size_t len) {
    string res = peek_output(len);
    pop_output(len);
    return res;
}

void ByteStream::end_input() { _end_input = true;}

bool ByteStream::input_ended() const { return _end_input; }

size_t ByteStream::buffer_size() const { return _capacity-freesize; }

bool ByteStream::buffer_empty() const { return freesize==_capacity; }

bool ByteStream::eof() const { return input_ended() && buffer_empty(); }

size_t ByteStream::bytes_written() const { return bytes_write_all; }

size_t ByteStream::bytes_read() const { return bytes_read_all; }

size_t ByteStream::remaining_capacity() const { return freesize; }
