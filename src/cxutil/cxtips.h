// Copyright (c) 2023 gk646
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
#define CX_FINISHED
#ifndef CXSTRUCTS_SRC_CXUTIL_CXTIPS_H_
#define CXSTRUCTS_SRC_CXUTIL_CXTIPS_H_

//Links provided without warranty of function, implied ownership or support of the linked website

//Extensive ranking of header compile times
//https://artificial-mind.net/projects/compile-health/

//
// Tips:
//
/// 1. Explicit is better than implicit (constructors)
//
/// 2. Use const when you can
//
/// 3. Use extra variables to keep every code line without line breaks
// const auto dx = 5;
// const auto dy = 10;
// DrawText(dx,dy,"Text");
//
/// 4. Use constexpr for inside function constants
//  constexpr int threshHold = 42;
//  if(val > threshHold){}
//
/// 5. Use anonymous namespace for internal linked function
//
// namespace{
// inline DoSomething(Foo bar){}
// }
//
// DoSomething(myBar)
//
///6. Use modern C++ features (generally)
//
// std::print({},3.0F); //instead of printf(%.f,3.0F);
//
//

#endif  //CXSTRUCTS_SRC_CXUTIL_CXTIPS_H_
