//
// Created by Admin on 28/03/2025.
//

#pragma once
 
 namespace My::MyECS {
 template<typename Cmpt>
 Span<Cmpt> ChunkView::GetCmptArray() const {
   auto* ptr = GetCmptArray(CmptType::Of<Cmpt>);
   if(ptr)
     return { static_cast<Cmpt*>(GetCmptArray(CmptType::Of<Cmpt>)), EntityNum() };
 }
 }
