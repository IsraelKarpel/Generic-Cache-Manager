
//
// Created by israelk on 01/12/2019.
//
#include <iostream>
#include <string>
#include <unordered_map>
#include <fstream>
#include <list>
#include <functional>

using namespace std;

template <class T>
class  CacheManager{
private:
    unordered_map<string, pair<T, std::list<string>::iterator>> _cache;
    list<string> keys;

    int size;
    int counter = 0;

public:
    CacheManager (int capicity) : size(capicity){}
    virtual ~CacheManager() {}

    void insert(string key, T obj) {
            ofstream fp;
            if (_cache.find(key) == _cache.end()) {
                //not found
                if (counter >= size) {
                    //pop the LRU
                    string temp = keys.back();
                    keys.pop_back();
                    _cache.erase(temp);
                }
                //insert to new key
                keys.push_front(key);
                _cache[key] = {obj, keys.begin()};
                counter++;
            } else {
                //found, update;
                keys.remove(key);
                keys.push_front(key);
                _cache[key] = {obj, keys.begin()};
            }
            //any way. write it in the file
            fp.open(key, ios::binary | ios::out);
            if (!fp.is_open()) {
                throw " error";
            }
            fp.write((char*) &obj, sizeof(obj));
            fp.close();
        }

    T get(string key) {
        T obj;
        ifstream fp;
        fp.open(key, ios::binary | ios::in);
        if (!fp.is_open()) {
            //the file isnt in the library, so he isnt in the cache also
            throw "error";
        } else {
            //found, return it
            fp.read((char*)&obj, sizeof(obj));
            if (counter >= size) {
                //if the returned obj is not in the list
                if (_cache.find(key) == _cache.end()) {
                    string temp = keys.back();
                    keys.pop_back();
                    _cache.erase(temp);
                    keys.push_front(key);
                    _cache[key] = {obj, keys.begin()};
                } else {
                    //the obj already in the cache
                    keys.remove(key);
                    keys.push_front(key);
                }
            } else {
                    keys.remove(key);
                    keys.push_front(key);

                }
            }
            _cache[key] = {obj, keys.begin()};
            return obj;

    }

    void foreach(const function <void(T&)> func){
        //for(auto iterator = _cache.; iterator != _cache.end(); iterator++) {
        for (string s: keys) {
            func(_cache[s].first);
        }
    }
};








