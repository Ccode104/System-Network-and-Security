# Number Theory Questions

## 1. Common Divisors
Given `n` integers \( \{m_i\}_{i=1}^{n} \), print all common divisors of \( (m_1, m_2, \dots, m_n) \) in ascending order.

**Example:**
```
Input: 3 12 18 36
Output: 1 2 3 6
```

## 2. Extended Euclidean Algorithm
Given integers `a` and `b`, output values `x` and `y` such that:
\[
ax + by = \gcd(a, b)\quad \text{where} \quad x < y
\]

## 3. Fundamental Theorem of Arithmetic
Given any integer, output its prime factorization in ascending order. If the same prime appears multiple times, print it that many times, separated by spaces.

## 4. Reduced Residue System Modulo `m`
Given an integer `m`, output the RRSM set \( RRSM_m \) and also the value of Euler’s Totient Function \( \phi(m) \).

## 5. Modular Exponentiation Using Fermat's Theorem
Given `a`, `x`, and `n`, compute:
\[
 a^x \mod n
\]
using Fermat's theorem. Also, print intermediate equations during computation.

## 6. Multiplicative Inverse
Given `a` and `m`, first determine if the multiplicative inverse of `a` modulo `m` exists (Y/N). If it exists, print the inverse.

## 7. Solutions of a Congruence
Given `a`, `b`, and `m`, determine if the congruence:
\[
 ax \equiv b \pmod{m}
\]
has a solution. If a solution exists, output the number of solutions and all solutions.

## 8. Solution to a System of Congruences
Given a set of integers \( \{(a_i, b_i, m_i)\}_{i=1}^{n} \), determine if a common solution `x` exists that satisfies the system of congruences:
\[
 a_i x \equiv b_i \pmod{m_i}
\]
If a solution exists, print all valid solutions. Use a user-defined Chinese Remainder Theorem (CRT) function.

## 9. Order of an Element Modulo `m`
Given `a` and `m`, print the order of `a` under modulo `m`.

## 10. Primitive Roots
Given `m`, compute and print the total number of primitive roots modulo `m`. Also, print all primitive roots under modulo `m`. 

