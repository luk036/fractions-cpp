use std::cmp::Ordering;
use std::fmt;
use std::hash::{Hash, Hasher};
use std::ops::*;
use std::str::FromStr;

#[derive(Debug, Clone, Copy)]
pub struct ExtFraction {
    numerator: i128,
    denominator: i128,
}

impl PartialEq for ExtFraction {
    fn eq(&self, other: &Self) -> bool {
        // NaN != NaN
        if self.is_nan() || other.is_nan() {
            return false;
        }

        // Both infinities with same sign are equal
        if self.is_infinite() && other.is_infinite() {
            return self.numerator == other.numerator;
        }

        // If one is infinite and the other is not, they're not equal
        if self.is_infinite() || other.is_infinite() {
            return false;
        }

        // Normal comparison
        self.numerator == other.numerator && self.denominator == other.denominator
    }
}

impl Eq for ExtFraction {}

impl ExtFraction {
    pub fn new(numerator: i128, denominator: i128) -> Self {
        if denominator == 0 {
            if numerator == 0 {
                ExtFraction {
                    numerator: 0,
                    denominator: 0,
                }
            } else if numerator > 0 {
                ExtFraction {
                    numerator: 1,
                    denominator: 0,
                }
            } else {
                ExtFraction {
                    numerator: -1,
                    denominator: 0,
                }
            }
        } else {
            let (num, den) = Self::reduce(numerator, denominator);
            ExtFraction {
                numerator: num,
                denominator: den,
            }
        }
    }

    pub fn from_integer(n: i128) -> Self {
        ExtFraction {
            numerator: n,
            denominator: 1,
        }
    }

    pub fn infinity(sign: bool) -> Self {
        ExtFraction {
            numerator: if sign { 1 } else { -1 },
            denominator: 0,
        }
    }

    pub fn nan() -> Self {
        ExtFraction {
            numerator: 0,
            denominator: 0,
        }
    }

    pub fn is_finite(&self) -> bool {
        self.denominator != 0
    }

    pub fn is_infinite(&self) -> bool {
        self.denominator == 0 && self.numerator != 0
    }

    pub fn is_nan(&self) -> bool {
        self.denominator == 0 && self.numerator == 0
    }

    pub fn from_float(f: f64) -> Self {
        if f.is_nan() {
            return Self::nan();
        }
        if f.is_infinite() {
            return Self::infinity(f.is_sign_positive());
        }
        let bits = f.to_bits();
        let sign = if bits >> 63 == 1 { -1i128 } else { 1 };
        let exponent = ((bits >> 52) & 0x7ff) as i32 - 1023;
        let mantissa = ((bits & ((1u64 << 52) - 1)) as i128) | (1i128 << 52);

        let numerator = sign * mantissa;
        let denominator = if exponent >= 0 {
            1i128.checked_shl(exponent as u32).unwrap_or(i128::MAX)
        } else {
            1i128
        };

        let (num, den) = if exponent >= 0 {
            (numerator, denominator)
        } else {
            (numerator, 1i128.checked_shl((-exponent) as u32).unwrap_or(i128::MAX))
        };

        ExtFraction {
            numerator: num,
            denominator: den,
        }
    }

    pub fn from_str(s: &str) -> Result<Self, String> {
        let s = s.trim().to_lowercase();
        match s.as_str() {
            "inf" | "+inf" | "infinity" | "+infinity" => Ok(Self::infinity(true)),
            "-inf" | "-infinity" => Ok(Self::infinity(false)),
            "nan" => Ok(Self::nan()),
            _ => {
                if s.contains('/') {
                    let parts: Vec<&str> = s.split('/').collect();
                    if parts.len() != 2 {
                        return Err("Invalid fraction format".to_string());
                    }
                    let num: i128 = parts[0].parse().map_err(|_| "Invalid numerator")?;
                    let den: i128 = parts[1].parse().map_err(|_| "Invalid denominator")?;
                    Ok(Self::new(num, den))
                } else if s.contains('.') {
                    let f: f64 = s.parse().map_err(|_| "Invalid float")?;
                    Ok(Self::from_float(f))
                } else {
                    let num: i128 = s.parse().map_err(|_| "Invalid integer")?;
                    Ok(ExtFraction::from_integer(num))
                }
            }
        }
    }

    fn reduce(numerator: i128, denominator: i128) -> (i128, i128) {
        let mut num = numerator.abs();
        let mut den = denominator.abs();
        let g = gcd(num, den);
        num /= g;
        den /= g;

        // The sign should be negative if numerator and denominator have different signs
        if (numerator < 0) != (denominator < 0) {
            (-num, den)
        } else {
            (num, den)
        }
    }

    pub fn numerator(&self) -> i128 {
        self.numerator
    }

    pub fn denominator(&self) -> i128 {
        self.denominator
    }

    pub fn is_integer(&self) -> bool {
        self.denominator == 1
    }

    pub fn as_integer_ratio(&self) -> (i128, i128) {
        (self.numerator, self.denominator)
    }

    pub fn limit_denominator(&self, max_denominator: i128) -> Result<Self, String> {
        if self.is_nan() || self.is_infinite() {
            return Ok(*self);
        }
        if max_denominator < 1 {
            return Err("max_denominator should be at least 1".to_string());
        }
        if self.denominator <= max_denominator {
            return Ok(*self);
        }

        let mut p0: i128 = 0;
        let mut q0: i128 = 1;
        let mut p1: i128 = 1;
        let mut q1: i128 = 0;
        let mut n = self.numerator;
        let mut d = self.denominator;

        loop {
            let a = n / d;
            let q2 = q0 + a * q1;
            if q2 > max_denominator {
                break;
            }
            let new_p0 = p1;
            let new_q0 = q1;
            let new_p1 = p0 + a * p1;
            let new_q1 = q2;
            p0 = new_p0;
            q0 = new_q0;
            p1 = new_p1;
            q1 = new_q1;
            let new_n = d;
            let new_d = n - a * d;
            n = new_n;
            d = new_d;
        }

        let k = (max_denominator - q0) / q1;

        if 2 * d * (q0 + k * q1) <= self.denominator {
            Ok(ExtFraction {
                numerator: p1,
                denominator: q1,
            })
        } else {
            Ok(ExtFraction {
                numerator: p0 + k * p1,
                denominator: q0 + k * q1,
            })
        }
    }

    fn add(self, other: ExtFraction) -> ExtFraction {
        // Handle special values
        if self.is_nan() || other.is_nan() {
            return Self::nan();
        }
        if self.is_infinite() && other.is_infinite() {
            return Self::nan();
        }
        if self.is_infinite() || other.is_infinite() {
            return if self.is_infinite() { self } else { other };
        }

        let na = self.numerator;
        let da = self.denominator;
        let nb = other.numerator;
        let db = other.denominator;

        let g = gcd(da, db);
        if g == 1 {
            let (num, den) = Self::reduce(na * db + da * nb, da * db);
            return ExtFraction { numerator: num, denominator: den };
        }

        let s = da / g;
        let t = na * (db / g) + nb * s;
        let g2 = gcd(t, g);

        if g2 == 1 {
            let (num, den) = Self::reduce(t, s * db);
            ExtFraction { numerator: num, denominator: den }
        } else {
            let (num, den) = Self::reduce(t / g2, s * (db / g2));
            ExtFraction { numerator: num, denominator: den }
        }
    }

    fn sub(self, other: ExtFraction) -> ExtFraction {
        // Handle special values
        if self.is_nan() || other.is_nan() {
            return Self::nan();
        }
        if self.is_infinite() && other.is_infinite() {
            // Same sign infinities cancel to NaN
            if self.numerator == other.numerator {
                return Self::nan();
            }
            // Different signs keep the larger infinity
            return self;
        }
        if self.is_infinite() || other.is_infinite() {
            return if self.is_infinite() { self } else { -other };
        }

        let na = self.numerator;
        let da = self.denominator;
        let nb = other.numerator;
        let db = other.denominator;

        let g = gcd(da, db);
        if g == 1 {
            let (num, den) = Self::reduce(na * db - da * nb, da * db);
            return ExtFraction { numerator: num, denominator: den };
        }

        let s = da / g;
        let t = na * (db / g) - nb * s;
        let g2 = gcd(t, g);

        if g2 == 1 {
            let (num, den) = Self::reduce(t, s * db);
            ExtFraction { numerator: num, denominator: den }
        } else {
            let (num, den) = Self::reduce(t / g2, s * (db / g2));
            ExtFraction { numerator: num, denominator: den }
        }
    }

    fn mul(self, other: ExtFraction) -> ExtFraction {
        // Handle special values
        if self.is_nan() || other.is_nan() {
            return Self::nan();
        }
        if self.is_infinite() || other.is_infinite() {
            // 0 * infinity = NaN
            if (self.is_infinite() && other.numerator == 0) || (other.is_infinite() && self.numerator == 0) {
                return Self::nan();
            }
            // Determine sign
            let sign_positive = if self.is_infinite() {
                self.numerator > 0
            } else {
                other.numerator > 0
            };
            let other_sign_positive = if self.is_infinite() {
                other.numerator >= 0
            } else {
                self.numerator >= 0
            };
            return Self::infinity(sign_positive == other_sign_positive);
        }

        let mut na = self.numerator;
        let mut da = self.denominator;
        let mut nb = other.numerator;
        let mut db = other.denominator;

        let g1 = gcd(na, db);
        if g1 > 1 {
            na /= g1;
            db /= g1;
        }

        let g2 = gcd(nb, da);
        if g2 > 1 {
            nb /= g2;
            da /= g2;
        }

        let (num, den) = Self::reduce(na * nb, db * da);
        ExtFraction { numerator: num, denominator: den }
    }

    fn div(self, other: ExtFraction) -> ExtFraction {
        // Handle special values
        if self.is_nan() || other.is_nan() {
            return Self::nan();
        }
        if other.is_infinite() {
            // finite / infinity = 0
            return Self::from_integer(0);
        }
        if self.is_infinite() {
            // infinity / finite = infinity (with sign)
            return self;
        }
        if other.numerator == 0 {
            // Division by zero
            if self.numerator == 0 {
                return Self::nan();
            }
            return Self::infinity(self.numerator > 0);
        }

        let mut na = self.numerator;
        let mut da = self.denominator;
        let mut nb = other.numerator;
        let mut db = other.denominator;

        let g1 = gcd(na, nb);
        if g1 > 1 {
            na /= g1;
            nb /= g1;
        }

        let g2 = gcd(db, da);
        if g2 > 1 {
            da /= g2;
            db /= g2;
        }

        let mut num = na * db;
        let mut den = nb * da;

        if den < 0 {
            num = -num;
            den = -den;
        }

        let (reduced_num, reduced_den) = Self::reduce(num, den);
        ExtFraction {
            numerator: reduced_num,
            denominator: reduced_den,
        }
    }

    pub fn pow(&self, exp: i32) -> ExtFraction {
        if self.is_nan() {
            return Self::nan();
        }
        if self.is_infinite() {
            if exp == 0 {
                return Self::from_integer(1);
            }
            // infinity^positive = infinity, infinity^negative = 0
            if exp > 0 {
                return *self;
            } else {
                return Self::from_integer(0);
            }
        }
        if exp >= 0 {
            let (num, den) = Self::reduce(
                self.numerator.pow(exp as u32),
                self.denominator.pow(exp as u32),
            );
            ExtFraction { numerator: num, denominator: den }
        } else {
            if self.numerator == 0 {
                return Self::infinity(true); // 0^(-n) = infinity
            }
            let (num, den) = Self::reduce(
                self.denominator.pow((-exp) as u32),
                self.numerator.pow((-exp) as u32),
            );
            ExtFraction { numerator: num, denominator: den }
        }
    }
}

fn gcd(a: i128, b: i128) -> i128 {
    let mut a = a.abs();
    let mut b = b.abs();
    while b != 0 {
        let t = b;
        b = a % b;
        a = t;
    }
    a
}

impl Add for ExtFraction {
    type Output = ExtFraction;

    fn add(self, other: Self) -> Self::Output {
        self.add(other)
    }
}

impl Add<i128> for ExtFraction {
    type Output = ExtFraction;

    fn add(self, other: i128) -> Self::Output {
        self + ExtFraction::from_integer(other)
    }
}

impl Add<ExtFraction> for i128 {
    type Output = ExtFraction;

    fn add(self, other: ExtFraction) -> Self::Output {
        ExtFraction::from_integer(self) + other
    }
}

impl Sub for ExtFraction {
    type Output = ExtFraction;

    fn sub(self, other: Self) -> Self::Output {
        self.sub(other)
    }
}

impl Sub<i128> for ExtFraction {
    type Output = ExtFraction;

    fn sub(self, other: i128) -> Self::Output {
        self - ExtFraction::from_integer(other)
    }
}

impl Sub<ExtFraction> for i128 {
    type Output = ExtFraction;

    fn sub(self, other: ExtFraction) -> Self::Output {
        ExtFraction::from_integer(self) - other
    }
}

impl Mul for ExtFraction {
    type Output = ExtFraction;

    fn mul(self, other: Self) -> Self::Output {
        self.mul(other)
    }
}

impl Mul<i128> for ExtFraction {
    type Output = ExtFraction;

    fn mul(self, other: i128) -> Self::Output {
        self * ExtFraction::from_integer(other)
    }
}

impl Mul<ExtFraction> for i128 {
    type Output = ExtFraction;

    fn mul(self, other: ExtFraction) -> Self::Output {
        ExtFraction::from_integer(self) * other
    }
}

impl Div for ExtFraction {
    type Output = ExtFraction;

    fn div(self, other: Self) -> Self::Output {
        self.div(other)
    }
}

impl Div<i128> for ExtFraction {
    type Output = ExtFraction;

    fn div(self, other: i128) -> Self::Output {
        self / ExtFraction::from_integer(other)
    }
}

impl Div<ExtFraction> for i128 {
    type Output = ExtFraction;

    fn div(self, other: ExtFraction) -> Self::Output {
        ExtFraction::from_integer(self) / other
    }
}

impl Neg for ExtFraction {
    type Output = ExtFraction;

    fn neg(self) -> Self::Output {
        if self.is_nan() {
            return Self::nan();
        }
        if self.is_infinite() {
            return Self::infinity(self.numerator < 0);
        }
        ExtFraction {
            numerator: -self.numerator,
            denominator: self.denominator,
        }
    }
}

impl PartialOrd for ExtFraction {
    fn partial_cmp(&self, other: &Self) -> Option<Ordering> {
        // NaN comparisons return None
        if self.is_nan() || other.is_nan() {
            return None;
        }

        // Handle infinities
        if self.is_infinite() && other.is_infinite() {
            if self.numerator == other.numerator {
                return Some(Ordering::Equal);
            }
            return Some(if self.numerator > other.numerator {
                Ordering::Greater
            } else {
                Ordering::Less
            });
        }
        if self.is_infinite() {
            return Some(if self.numerator > 0 {
                Ordering::Greater
            } else {
                Ordering::Less
            });
        }
        if other.is_infinite() {
            return Some(if other.numerator > 0 {
                Ordering::Less
            } else {
                Ordering::Greater
            });
        }

        // Normal comparison
        Some(self.cmp(other))
    }
}

impl Ord for ExtFraction {
    fn cmp(&self, other: &Self) -> Ordering {
        (self.numerator * other.denominator).cmp(&(other.numerator * self.denominator))
    }
}

impl Hash for ExtFraction {
    fn hash<H: Hasher>(&self, state: &mut H) {
        self.numerator.hash(state);
        self.denominator.hash(state);
    }
}

impl fmt::Display for ExtFraction {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        if self.denominator == 0 {
            if self.numerator == 0 {
                write!(f, "nan")
            } else if self.numerator > 0 {
                write!(f, "inf")
            } else {
                write!(f, "-inf")
            }
        } else if self.denominator == 1 {
            write!(f, "{}", self.numerator)
        } else {
            write!(f, "{}/{}", self.numerator, self.denominator)
        }
    }
}

impl FromStr for ExtFraction {
    type Err = String;

    fn from_str(s: &str) -> Result<Self, Self::Err> {
        ExtFraction::from_str(s)
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_creation() {
        let f = ExtFraction::new(4, 3);
        assert_eq!(f.numerator(), 4);
        assert_eq!(f.denominator(), 3);

        let f = ExtFraction::new(8, 6);
        assert_eq!(f.numerator(), 4);
        assert_eq!(f.denominator(), 3);

        let f = ExtFraction::new(-5, 4);
        assert_eq!(f.numerator(), -5);
        assert_eq!(f.denominator(), 4);
    }

    #[test]
    fn test_zero_denominator() {
        let f = ExtFraction::new(1, 0);
        assert!(f.is_infinite());
        assert!(!f.is_finite());
    }

    #[test]
    fn test_from_integer() {
        let f = ExtFraction::from_integer(5);
        assert_eq!(f.numerator(), 5);
        assert_eq!(f.denominator(), 1);
    }

    #[test]
    fn test_addition() {
        let f1 = ExtFraction::new(1, 2);
        let f2 = ExtFraction::new(1, 3);
        let result = f1 + f2;
        assert_eq!(result.numerator(), 5);
        assert_eq!(result.denominator(), 6);
    }

    #[test]
    fn test_subtraction() {
        let f1 = ExtFraction::new(3, 4);
        let f2 = ExtFraction::new(1, 4);
        let result = f1 - f2;
        assert_eq!(result.numerator(), 1);
        assert_eq!(result.denominator(), 2);
    }

    #[test]
    fn test_multiplication() {
        let f1 = ExtFraction::new(2, 3);
        let f2 = ExtFraction::new(3, 4);
        let result = f1 * f2;
        assert_eq!(result.numerator(), 1);
        assert_eq!(result.denominator(), 2);
    }

    #[test]
    fn test_division() {
        let f1 = ExtFraction::new(3, 4);
        let f2 = ExtFraction::new(1, 2);
        let result = f1 / f2;
        assert_eq!(result.numerator(), 3);
        assert_eq!(result.denominator(), 2);
    }

    #[test]
    fn test_division_by_zero() {
        let f1 = ExtFraction::new(1, 2);
        let f2 = ExtFraction::new(0, 1);
        let result = f1 / f2;
        assert!(result.is_infinite());
    }

    #[test]
    fn test_negation() {
        let f = ExtFraction::new(3, 4);
        let result = -f;
        assert_eq!(result.numerator(), -3);
        assert_eq!(result.denominator(), 4);
    }

    #[test]
    fn test_comparison() {
        let f1 = ExtFraction::new(1, 2);
        let f2 = ExtFraction::new(1, 3);
        assert!(f1 > f2);
        assert!(f2 < f1);
        assert!(f1 >= f2);
        assert!(f2 <= f1);
        assert_eq!(f1, ExtFraction::new(2, 4));
    }

    #[test]
    fn test_display() {
        let f = ExtFraction::new(3, 4);
        assert_eq!(format!("{}", f), "3/4");

        let f = ExtFraction::new(5, 1);
        assert_eq!(format!("{}", f), "5");
    }

    #[test]
    fn test_from_str() {
        let f = ExtFraction::from_str("3/4").unwrap();
        assert_eq!(f.numerator(), 3);
        assert_eq!(f.denominator(), 4);

        let f = ExtFraction::from_str("5").unwrap();
        assert_eq!(f.numerator(), 5);
        assert_eq!(f.denominator(), 1);
    }

    #[test]
    fn test_pow() {
        let f = ExtFraction::new(2, 3);
        let result = f.pow(2);
        assert_eq!(result.numerator(), 4);
        assert_eq!(result.denominator(), 9);

        let result = f.pow(-1);
        assert_eq!(result.numerator(), 3);
        assert_eq!(result.denominator(), 2);
    }

    #[test]
    fn test_limit_denominator() {
        let f = ExtFraction::new(22, 7);
        let result = f.limit_denominator(10).unwrap();
        assert_eq!(result.numerator(), 22);
        assert_eq!(result.denominator(), 7);
    }

    #[test]
    fn test_is_integer() {
        let f = ExtFraction::new(4, 2);
        assert!(f.is_integer());

        let f = ExtFraction::new(3, 4);
        assert!(!f.is_integer());
    }

    #[test]
    fn test_special_values() {
        let zero = ExtFraction::from_integer(0);
        let one = ExtFraction::from_integer(1);

        // Positive infinity
        let pos_inf = one / zero;
        assert!(pos_inf.is_infinite());
        assert!(!pos_inf.is_finite());
        assert!(!pos_inf.is_nan());
        assert_eq!(format!("{}", pos_inf), "inf");

        // Negative infinity
        let neg_inf = -one / zero;
        assert!(neg_inf.is_infinite());
        assert_eq!(format!("{}", neg_inf), "-inf");

        // NaN
        let nan = zero / zero;
        assert!(nan.is_nan());
        assert!(!nan.is_finite());
        assert!(!nan.is_infinite());
        assert_eq!(format!("{}", nan), "nan");

        // NaN is not equal to itself
        assert!(nan != nan);

        // Infinity comparisons
        assert!(pos_inf > ExtFraction::from_integer(1000));
        assert!(neg_inf < ExtFraction::from_integer(-1000));
        assert!(pos_inf > neg_inf);
        assert_eq!(pos_inf, pos_inf);
        assert_eq!(neg_inf, neg_inf);
    }

    #[test]
    fn test_infinity_operations() {
        let zero = ExtFraction::from_integer(0);
        let one = ExtFraction::from_integer(1);
        let two = ExtFraction::from_integer(2);
        let inf = one / zero;

        // Addition
        assert_eq!(inf + two, inf);
        assert_eq!(two + inf, inf);

        // Subtraction
        assert_eq!(inf - two, inf);
        assert!((inf - inf).is_nan());

        // Multiplication
        assert_eq!(inf * two, inf);
        assert!((zero * inf).is_nan());

        // Division
        assert_eq!(inf / two, inf);
        assert_eq!(two / inf, zero);
    }

    #[test]
    fn test_from_float_special() {
        let pos_inf = ExtFraction::from_float(f64::INFINITY);
        assert!(pos_inf.is_infinite());
        assert_eq!(format!("{}", pos_inf), "inf");

        let neg_inf = ExtFraction::from_float(f64::NEG_INFINITY);
        assert!(neg_inf.is_infinite());
        assert_eq!(format!("{}", neg_inf), "-inf");

        let nan = ExtFraction::from_float(f64::NAN);
        assert!(nan.is_nan());
        assert_eq!(format!("{}", nan), "nan");
    }

    #[test]
    fn test_from_str_special() {
        let pos_inf = ExtFraction::from_str("inf").unwrap();
        assert!(pos_inf.is_infinite());

        let neg_inf = ExtFraction::from_str("-inf").unwrap();
        assert!(neg_inf.is_infinite());

        let nan = ExtFraction::from_str("nan").unwrap();
        assert!(nan.is_nan());
    }
}

#[cfg(test)]
mod proptests {
    use super::*;
    use proptest::prelude::*;

    proptest! {
        #[test]
        fn prop_addition_commutative(a in -1000i128..1000, b in -1000i128..1000) {
            if b != 0 {
                let f1 = ExtFraction::new(a, b);
                let f2 = ExtFraction::new(b, a.abs() + 1);
                prop_assert_eq!(f1 + f2, f2 + f1);
            }
        }

        #[test]
        fn prop_addition_associative(
            a in -100i128..100,
            b in -100i128..100,
            c in -100i128..100,
            d in -100i128..100,
            e in 1i128..100,
            f in 1i128..100,
            g in 1i128..100
        ) {
            let f1 = ExtFraction::new(a, e);
            let f2 = ExtFraction::new(b, f);
            let f3 = ExtFraction::new(c, g);
            prop_assert_eq!((f1 + f2) + f3, f1 + (f2 + f3));
        }

        #[test]
        fn prop_multiplication_commutative(a in -1000i128..1000, b in -1000i128..1000) {
            if b != 0 && a != 0 {
                let f1 = ExtFraction::new(a, b);
                let f2 = ExtFraction::new(b, a.abs() + 1);
                prop_assert_eq!(f1 * f2, f2 * f1);
            }
        }

        #[test]
        fn prop_multiplication_associative(
            a in -100i128..100,
            b in -100i128..100,
            c in -100i128..100,
            d in -100i128..100,
            e in 1i128..100,
            f in 1i128..100,
            g in 1i128..100
        ) {
            let f1 = ExtFraction::new(a, e);
            let f2 = ExtFraction::new(b, f);
            let f3 = ExtFraction::new(c, g);
            prop_assert_eq!((f1 * f2) * f3, f1 * (f2 * f3));
        }

        #[test]
        fn prop_distributive_law(
            a in -100i128..100,
            b in -100i128..100,
            c in -100i128..100,
            d in -100i128..100,
            e in -100i128..100,
            f in -100i128..100,
            g in 1i128..100,
            h in 1i128..100,
            i in 1i128..100
        ) {
            let f1 = ExtFraction::new(a, g);
            let f2 = ExtFraction::new(b, h);
            let f3 = ExtFraction::new(c, i);
            prop_assert_eq!(f1 * (f2 + f3), f1 * f2 + f1 * f3);
        }

        #[test]
        fn prop_additive_identity(a in -1000i128..1000, b in 1i128..1000) {
            let f = ExtFraction::new(a, b);
            let zero = ExtFraction::from_integer(0);
            prop_assert_eq!(f + zero, f);
            prop_assert_eq!(zero + f, f);
        }

        #[test]
        fn prop_multiplicative_identity(a in -1000i128..1000, b in 1i128..1000) {
            let f = ExtFraction::new(a, b);
            let one = ExtFraction::from_integer(1);
            prop_assert_eq!(f * one, f);
            prop_assert_eq!(one * f, f);
        }

        #[test]
        fn prop_additive_inverse(a in -1000i128..1000, b in 1i128..1000) {
            let f = ExtFraction::new(a, b);
            let zero = ExtFraction::from_integer(0);
            prop_assert_eq!(f + (-f), zero);
        }

        #[test]
        fn prop_multiplicative_inverse(a in -1000i128..1000, b in 1i128..1000) {
            if a != 0 {
                let f = ExtFraction::new(a, b);
                let one = ExtFraction::from_integer(1);
                prop_assert_eq!(f * (ExtFraction::from_integer(1) / f), one);
            }
        }

        #[test]
        fn prop_subtraction_definition(a in -1000i128..1000, b in 1i128..1000, c in -1000i128..1000, d in 1i128..1000) {
            let f1 = ExtFraction::new(a, b);
            let f2 = ExtFraction::new(c, d);
            prop_assert_eq!(f1 - f2, f1 + (-f2));
        }

        #[test]
        fn prop_division_definition(a in -1000i128..1000, b in 1i128..1000, c in -1000i128..1000, d in 1i128..1000) {
            if c != 0 {
                let f1 = ExtFraction::new(a, b);
                let f2 = ExtFraction::new(c, d);
                prop_assert_eq!(f1 / f2, f1 * (ExtFraction::from_integer(1) / f2));
            }
        }

        #[test]
        fn prop_reflexive_equality(a in -1000i128..1000, b in 1i128..1000) {
            let f = ExtFraction::new(a, b);
            prop_assert!(f == f);
        }

        #[test]
        fn prop_symmetric_equality(a in -1000i128..1000, b in 1i128..1000, c in -1000i128..1000, d in 1i128..1000) {
            let f1 = ExtFraction::new(a, b);
            let f2 = ExtFraction::new(c, d);
            if f1 == f2 {
                prop_assert!(f2 == f1);
            }
        }

        #[test]
        fn prop_transitive_equality(
            a in -100i128..100, b in 1i128..100,
            c in -100i128..100, d in 1i128..100,
            e in -100i128..100, f in 1i128..100
        ) {
            let f1 = ExtFraction::new(a, b);
            let f2 = ExtFraction::new(c, d);
            let f3 = ExtFraction::new(e, f);
            if f1 == f2 && f2 == f3 {
                prop_assert!(f1 == f3);
            }
        }

        #[test]
        fn prop_comparison_transitivity(
            a in -100i128..100, b in 1i128..100,
            c in -100i128..100, d in 1i128..100,
            e in -100i128..100, f in 1i128..100
        ) {
            let f1 = ExtFraction::new(a, b);
            let f2 = ExtFraction::new(c, d);
            let f3 = ExtFraction::new(e, f);
            if f1 < f2 && f2 < f3 {
                prop_assert!(f1 < f3);
            }
        }

        #[test]
        fn prop_comparison_totality(a in -1000i128..1000, b in 1i128..1000, c in -1000i128..1000, d in 1i128..1000) {
            let f1 = ExtFraction::new(a, b);
            let f2 = ExtFraction::new(c, d);
            if !f1.is_nan() && !f2.is_nan() {
                prop_assert!(f1 == f2 || f1 < f2 || f1 > f2);
            }
        }

        #[test]
        fn prop_pow_positive_exponent(a in -10i128..10, b in 1i128..10, exp in 0u8..5) {
            if a != 0 {
                let f = ExtFraction::new(a, b);
                let result = f.pow(exp as i32);
                let expected = (0..exp).fold(ExtFraction::from_integer(1), |acc, _| acc * f);
                prop_assert_eq!(result, expected);
            }
        }

        #[test]
        fn prop_pow_negative_exponent(a in -10i128..10, b in 1i128..10, exp in 1u8..5) {
            if a != 0 {
                let f = ExtFraction::new(a, b);
                let result = f.pow(-(exp as i32));
                let reciprocal = ExtFraction::from_integer(1) / f;
                let expected = (0..exp).fold(ExtFraction::from_integer(1), |acc, _| acc * reciprocal);
                prop_assert_eq!(result, expected);
            }
        }

        #[test]
        fn prop_pow_zero_exponent(a in -1000i128..1000, b in 1i128..1000) {
            let f = ExtFraction::new(a, b);
            prop_assert_eq!(f.pow(0), ExtFraction::from_integer(1));
        }

        #[test]
        fn prop_negation_twice(a in -1000i128..1000, b in 1i128..1000) {
            let f = ExtFraction::new(a, b);
            prop_assert_eq!(-(-f), f);
        }

        #[test]
        fn prop_integer_multiplication(a in -1000i128..1000, b in 1i128..1000, n in -1000i128..1000) {
            let f = ExtFraction::new(a, b);
            let f_int = ExtFraction::from_integer(n);
            prop_assert_eq!(f * n, f * f_int);
            prop_assert_eq!(n * f, f_int * f);
        }

        #[test]
        fn prop_integer_addition(a in -1000i128..1000, b in 1i128..1000, n in -1000i128..1000) {
            let f = ExtFraction::new(a, b);
            let f_int = ExtFraction::from_integer(n);
            prop_assert_eq!(f + n, f + f_int);
            prop_assert_eq!(n + f, f_int + f);
        }

        #[test]
        fn prop_integer_subtraction(a in -1000i128..1000, b in 1i128..1000, n in -1000i128..1000) {
            let f = ExtFraction::new(a, b);
            let f_int = ExtFraction::from_integer(n);
            prop_assert_eq!(f - n, f - f_int);
            prop_assert_eq!(n - f, f_int - f);
        }

        #[test]
        fn prop_integer_division(a in -1000i128..1000, b in 1i128..1000, n in -1000i128..1000) {
            if n != 0 {
                let f = ExtFraction::new(a, b);
                let f_int = ExtFraction::from_integer(n);
                prop_assert_eq!(f / n, f / f_int);
                prop_assert_eq!(n / f, f_int / f);
            }
        }

        #[test]
        fn prop_is_integer_consistency(a in -1000i128..1000, b in 1i128..1000) {
            let f = ExtFraction::new(a, b);
            if f.is_integer() {
                prop_assert_eq!(f.denominator(), 1);
            }
            if f.denominator() == 1 {
                prop_assert!(f.is_integer());
            }
        }
    }
}