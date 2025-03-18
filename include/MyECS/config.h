//
// Created by Admin on 16/03/2025.
//

#pragma once
 
 namespace My::MyECS {
 // compile time config
 
 // Cache-Lines size is (typically) 64 bytes
 // it has to be an exponent of 2 and >= 16
 constexpr size_t CHUNK_ALIGNMENT = 64;
 
 // 16384 bytes : 16 KB
 // it has to be a multiple of MYECS_CHUNK_ALIGNMENT and an exponent of 2
 constexpr size_t CHUNK_SIZE = 16384;
 }
