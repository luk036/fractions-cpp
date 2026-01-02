use std::cmp::Ordering;
use std::fmt;
use std::hash::{Hash, Hasher};
use std::ops::*;
use std::str::FromStr;

#[derive(Debug, Clone, Copy)]
pub struct Fraction {
    numerator: i128,
    denominator: i128,
}

impl PartialEq for Fraction {
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

impl Eq for Fraction {}

impl Fraction {
    pub fn new(numerator: i128, denominator: i128) -> Self {
        if denominator == 0 {
            if numerator == 0 {
                Fraction {
                    numerator: 0,
                    denominator: 0,
                }
            } else if numerator > 0 {
                Fraction {
                    numerator: 1,
                    denominator: 0,
                }
            } else {
                Fraction {
                    numerator: -1,
                    denominator: 0,
                }
            }
        } else {
            let (num, den) = Self::reduce(numerator, denominator);
            Fraction {
                numerator: num,
                denominator: den,
            }
        }
    }

    pub fn from_integer(n: i128) -> Self {
        Fraction {
            numerator: n,
            denominator: 1,
        }
    }

    pub fn infinity(sign: bool) -> Self {
        Fraction {
            numerator: if sign { 1 } else { -1 },
            denominator: 0,
        }
    }

    pub fn nan() -> Self {
        Fraction {
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
            1i128 << exponent
        } else {
            1i128
        };

        let (num, den) = if exponent >= 0 {
            (numerator, denominator)
        } else {
            (numerator, 1i128 << (-exponent))
        };

        Fraction {
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
                    Ok(Fraction::from_integer(num))
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
            Ok(Fraction {
                numerator: p1,
                denominator: q1,
            })
        } else {
            Ok(Fraction {
                numerator: p0 + k * p1,
                denominator: q0 + k * q1,
            })
        }
    }

    fn add(self, other: Fraction) -> Fraction {
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
            return Fraction { numerator: num, denominator: den };
        }

        let s = da / g;
        let t = na * (db / g) + nb * s;
        let g2 = gcd(t, g);

        if g2 == 1 {
            let (num, den) = Self::reduce(t, s * db);
            Fraction { numerator: num, denominator: den }
        } else {
            let (num, den) = Self::reduce(t / g2, s * (db / g2));
            Fraction { numerator: num, denominator: den }
        }
    }

    fn sub(self, other: Fraction) -> Fraction {
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
            return Fraction { numerator: num, denominator: den };
        }

        let s = da / g;
        let t = na * (db / g) - nb * s;
        let g2 = gcd(t, g);

        if g2 == 1 {
            let (num, den) = Self::reduce(t, s * db);
            Fraction { numerator: num, denominator: den }
        } else {
            let (num, den) = Self::reduce(t / g2, s * (db / g2));
            Fraction { numerator: num, denominator: den }
        }
    }

    fn mul(self, other: Fraction) -> Fraction {
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
        Fraction { numerator: num, denominator: den }
    }

    fn div(self, other: Fraction) -> Fraction {
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
        Fraction {
            numerator: reduced_num,
            denominator: reduced_den,
        }
    }

    pub fn pow(&self, exp: i32) -> Fraction {
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
            Fraction { numerator: num, denominator: den }
        } else {
            if self.numerator == 0 {
                return Self::infinity(true); // 0^(-n) = infinity
            }
            let (num, den) = Self::reduce(
                self.denominator.pow((-exp) as u32),
                self.numerator.pow((-exp) as u32),
            );
            Fraction { numerator: num, denominator: den }
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

impl Add for Fraction {
    type Output = Fraction;

    fn add(self, other: Self) -> Self::Output {
        self.add(other)
    }
}

impl Add<i128> for Fraction {
    type Output = Fraction;

    fn add(self, other: i128) -> Self::Output {
        self + Fraction::from_integer(other)
    }
}

impl Add<Fraction> for i128 {
    type Output = Fraction;

    fn add(self, other: Fraction) -> Self::Output {
        Fraction::from_integer(self) + other
    }
}

impl Sub for Fraction {
    type Output = Fraction;

    fn sub(self, other: Self) -> Self::Output {
        self.sub(other)
    }
}

impl Sub<i128> for Fraction {
    type Output = Fraction;

    fn sub(self, other: i128) -> Self::Output {
        self - Fraction::from_integer(other)
    }
}

impl Sub<Fraction> for i128 {
    type Output = Fraction;

    fn sub(self, other: Fraction) -> Self::Output {
        Fraction::from_integer(self) - other
    }
}

impl Mul for Fraction {
    type Output = Fraction;

    fn mul(self, other: Self) -> Self::Output {
        self.mul(other)
    }
}

impl Mul<i128> for Fraction {
    type Output = Fraction;

    fn mul(self, other: i128) -> Self::Output {
        self * Fraction::from_integer(other)
    }
}

impl Mul<Fraction> for i128 {
    type Output = Fraction;

    fn mul(self, other: Fraction) -> Self::Output {
        Fraction::from_integer(self) * other
    }
}

impl Div for Fraction {
    type Output = Fraction;

    fn div(self, other: Self) -> Self::Output {
        self.div(other)
    }
}

impl Div<i128> for Fraction {
    type Output = Fraction;

    fn div(self, other: i128) -> Self::Output {
        self / Fraction::from_integer(other)
    }
}

impl Div<Fraction> for i128 {
    type Output = Fraction;

    fn div(self, other: Fraction) -> Self::Output {
        Fraction::from_integer(self) / other
    }
}

impl Neg for Fraction {
    type Output = Fraction;

    fn neg(self) -> Self::Output {
        if self.is_nan() {
            return Self::nan();
        }
        if self.is_infinite() {
            return Self::infinity(self.numerator < 0);
        }
        Fraction {
            numerator: -self.numerator,
            denominator: self.denominator,
        }
    }
}

impl PartialOrd for Fraction {
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

impl Ord for Fraction {
    fn cmp(&self, other: &Self) -> Ordering {
        (self.numerator * other.denominator).cmp(&(other.numerator * self.denominator))
    }
}

impl Hash for Fraction {
    fn hash<H: Hasher>(&self, state: &mut H) {
        self.numerator.hash(state);
        self.denominator.hash(state);
    }
}

impl fmt::Display for Fraction {
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

impl FromStr for Fraction {
    type Err = String;

    fn from_str(s: &str) -> Result<Self, Self::Err> {
        Fraction::from_str(s)
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_creation() {
        let f = Fraction::new(4, 3);
        assert_eq!(f.numerator(), 4);
        assert_eq!(f.denominator(), 3);

        let f = Fraction::new(8, 6);
        assert_eq!(f.numerator(), 4);
        assert_eq!(f.denominator(), 3);

        let f = Fraction::new(-5, 4);
        assert_eq!(f.numerator(), -5);
        assert_eq!(f.denominator(), 4);
    }

    #[test]
    fn test_zero_denominator() {
        let f = Fraction::new(1, 0);
        assert!(f.is_infinite());
        assert!(!f.is_finite());
    }

    #[test]
    fn test_from_integer() {
        let f = Fraction::from_integer(5);
        assert_eq!(f.numerator(), 5);
        assert_eq!(f.denominator(), 1);
    }

    #[test]
    fn test_addition() {
        let f1 = Fraction::new(1, 2);
        let f2 = Fraction::new(1, 3);
        let result = f1 + f2;
        assert_eq!(result.numerator(), 5);
        assert_eq!(result.denominator(), 6);
    }

    #[test]
    fn test_subtraction() {
        let f1 = Fraction::new(3, 4);
        let f2 = Fraction::new(1, 4);
        let result = f1 - f2;
        assert_eq!(result.numerator(), 1);
        assert_eq!(result.denominator(), 2);
    }

    #[test]
    fn test_multiplication() {
        let f1 = Fraction::new(2, 3);
        let f2 = Fraction::new(3, 4);
        let result = f1 * f2;
        assert_eq!(result.numerator(), 1);
        assert_eq!(result.denominator(), 2);
    }

    #[test]
    fn test_division() {
        let f1 = Fraction::new(3, 4);
        let f2 = Fraction::new(1, 2);
        let result = f1 / f2;
        assert_eq!(result.numerator(), 3);
        assert_eq!(result.denominator(), 2);
    }

    #[test]
    fn test_division_by_zero() {
        let f1 = Fraction::new(1, 2);
        let f2 = Fraction::new(0, 1);
        let result = f1 / f2;
        assert!(result.is_infinite());
    }

    #[test]
    fn test_negation() {
        let f = Fraction::new(3, 4);
        let result = -f;
        assert_eq!(result.numerator(), -3);
        assert_eq!(result.denominator(), 4);
    }

    #[test]
    fn test_comparison() {
        let f1 = Fraction::new(1, 2);
        let f2 = Fraction::new(1, 3);
        assert!(f1 > f2);
        assert!(f2 < f1);
        assert!(f1 >= f2);
        assert!(f2 <= f1);
        assert_eq!(f1, Fraction::new(2, 4));
    }

    #[test]
    fn test_display() {
        let f = Fraction::new(3, 4);
        assert_eq!(format!("{}", f), "3/4");

        let f = Fraction::new(5, 1);
        assert_eq!(format!("{}", f), "5");
    }

    #[test]
    fn test_from_str() {
        let f = Fraction::from_str("3/4").unwrap();
        assert_eq!(f.numerator(), 3);
        assert_eq!(f.denominator(), 4);

        let f = Fraction::from_str("5").unwrap();
        assert_eq!(f.numerator(), 5);
        assert_eq!(f.denominator(), 1);
    }

    #[test]
    fn test_pow() {
        let f = Fraction::new(2, 3);
        let result = f.pow(2);
        assert_eq!(result.numerator(), 4);
        assert_eq!(result.denominator(), 9);

        let result = f.pow(-1);
        assert_eq!(result.numerator(), 3);
        assert_eq!(result.denominator(), 2);
    }

    #[test]
    fn test_limit_denominator() {
        let f = Fraction::new(22, 7);
        let result = f.limit_denominator(10).unwrap();
        assert_eq!(result.numerator(), 22);
        assert_eq!(result.denominator(), 7);
    }

    #[test]
    fn test_is_integer() {
        let f = Fraction::new(4, 2);
        assert!(f.is_integer());

        let f = Fraction::new(3, 4);
        assert!(!f.is_integer());
    }

    #[test]
    fn test_special_values() {
        let zero = Fraction::from_integer(0);
        let one = Fraction::from_integer(1);

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
        assert!(pos_inf > Fraction::from_integer(1000));
        assert!(neg_inf < Fraction::from_integer(-1000));
        assert!(pos_inf > neg_inf);
        assert_eq!(pos_inf, pos_inf);
        assert_eq!(neg_inf, neg_inf);
    }

    #[test]
    fn test_infinity_operations() {
        let zero = Fraction::from_integer(0);
        let one = Fraction::from_integer(1);
        let two = Fraction::from_integer(2);
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
        let pos_inf = Fraction::from_float(f64::INFINITY);
        assert!(pos_inf.is_infinite());
        assert_eq!(format!("{}", pos_inf), "inf");

        let neg_inf = Fraction::from_float(f64::NEG_INFINITY);
        assert!(neg_inf.is_infinite());
        assert_eq!(format!("{}", neg_inf), "-inf");

        let nan = Fraction::from_float(f64::NAN);
        assert!(nan.is_nan());
        assert_eq!(format!("{}", nan), "nan");
    }

    #[test]
    fn test_from_str_special() {
        let pos_inf = Fraction::from_str("inf").unwrap();
        assert!(pos_inf.is_infinite());

        let neg_inf = Fraction::from_str("-inf").unwrap();
        assert!(neg_inf.is_infinite());

        let nan = Fraction::from_str("nan").unwrap();
        assert!(nan.is_nan());
    }
}