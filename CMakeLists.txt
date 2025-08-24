cmake_minimum_required(VERSION 3.31)
project(interpeter_cpp)

set(CMAKE_CXX_STANDARD 20)

add_executable(interpeter_cpp src/main.cpp
        src/Token.cpp
        src/Token.h
        src/Scanner.cpp
        src/Scanner.h
        src/lox.cpp
        src/lox.h
        src/Expr.h
        src/AstPrinter.cpp
        src/AstPrinter.h
        src/Obj.cpp
        src/Obj.h
        src/Parser.cpp
        src/Parser.h
        src/Interpreter.cpp
        src/Interpreter.h
        src/RuntimeError.cpp
        src/RuntimeError.h
        src/Stmt.h
        src/Env.cpp
        src/Env.h
        src/LoxCallable.h
        src/LoxFunction.cpp
        src/LoxFunction.h
        src/ClockCallable.h
        src/ClockCallable.h.cpp
        src/ReturnValue.cpp
        src/ReturnValue.h
        src/Resolver.cpp
        src/Resolver.h
        src/LoxClass.cpp
        src/LoxClass.h
        src/LoxInstance.cpp
        src/LoxInstance.h)
