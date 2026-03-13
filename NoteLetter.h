#pragma once

struct NoteLetter {
   inline static const int count = 12;

   enum Type {
      A, Ad, B, C, Cd, D, Dd, E, F, Fd, G, Gd
   };

   Type value;

   constexpr NoteLetter(int n) : value(static_cast<Type>(n % 12)) {}

   constexpr operator Type() const { return value; }

   const char* name() const;
};
