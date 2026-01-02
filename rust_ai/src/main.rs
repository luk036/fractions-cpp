use fractions::Fraction;

fn main() {
    // Basic usage examples
    println!("=== Fraction Library Demo ===\n");

    // Creating fractions
    let f1 = Fraction::new(3, 4);
    let f2 = Fraction::new(1, 2);
    println!("f1 = {}", f1);
    println!("f2 = {}", f2);

    // Arithmetic operations
    println!("\nArithmetic Operations:");
    println!("f1 + f2 = {}", f1 + f2);
    println!("f1 - f2 = {}", f1 - f2);
    println!("f1 * f2 = {}", f1 * f2);
    println!("f1 / f2 = {}", f1 / f2);
    println!("-f1 = {}", -f1);

    // Comparison
    println!("\nComparison:");
    println!("f1 > f2: {}", f1 > f2);
    println!("f1 == f2: {}", f1 == f2);

    // Power
    println!("\nPower:");
    println!("f1^2 = {}", f1.pow(2));
    println!("f1^-1 = {}", f1.pow(-1));

    // From string
    println!("\nFrom string:");
    let f3 = Fraction::from_str("5/6").unwrap();
    println!("Fraction::from_str(\"5/6\") = {}", f3);

    // Limit denominator
    println!("\nLimit denominator:");
    let pi_approx = Fraction::new(355, 113);
    let limited = pi_approx.limit_denominator(100).unwrap();
    println!("355/113 limited to denominator 100 = {}", limited);

    // Is integer
    println!("\nIs integer:");
    println!("4/2 is integer: {}", Fraction::new(4, 2).is_integer());
    println!("3/4 is integer: {}", Fraction::new(3, 4).is_integer());

    // Special values
    println!("\n=== Special Values ===");

    // Division by zero
    println!("\nDivision by zero:");
    let zero = Fraction::from_integer(0);
    let one = Fraction::from_integer(1);
    let pos_inf = one / zero;
    let neg_inf = -one / zero;
    println!("1 / 0 = {}", pos_inf);
    println!("-1 / 0 = {}", neg_inf);
    println!("is_finite(1/0): {}", pos_inf.is_finite());
    println!("is_infinite(1/0): {}", pos_inf.is_infinite());

    // NaN
    println!("\nNaN:");
    let nan = zero / zero;
    println!("0 / 0 = {}", nan);
    println!("is_nan(0/0): {}", nan.is_nan());
    println!("inf - inf = {}", pos_inf - pos_inf);
    println!("0 * inf = {}", zero * pos_inf);

    // Infinity operations
    println!("\nInfinity operations:");
    let two = Fraction::from_integer(2);
    println!("inf + 2 = {}", pos_inf + two);
    println!("inf - 2 = {}", pos_inf - two);
    println!("inf * 2 = {}", pos_inf * two);
    println!("inf / 2 = {}", pos_inf / two);
    println!("2 / inf = {}", two / pos_inf);
    println!("inf^2 = {}", pos_inf.pow(2));
    println!("inf^-1 = {}", pos_inf.pow(-1));

    // Comparisons with special values
    println!("\nComparisons with special values:");
    println!("inf > 1000: {}", pos_inf > Fraction::from_integer(1000));
    println!("-inf < -1000: {}", neg_inf < Fraction::from_integer(-1000));
    println!("inf == inf: {}", pos_inf == pos_inf);
    println!("nan == nan: {}", nan == nan);
    println!("inf > -inf: {}", pos_inf > neg_inf);

    // From float with special values
    println!("\nFrom float:");
    println!("Fraction::from_float(f64::INFINITY) = {}", Fraction::from_float(f64::INFINITY));
    println!("Fraction::from_float(f64::NEG_INFINITY) = {}", Fraction::from_float(f64::NEG_INFINITY));
    println!("Fraction::from_float(f64::NAN) = {}", Fraction::from_float(f64::NAN));

    // From string with special values
    println!("\nFrom string:");
    println!("Fraction::from_str(\"inf\") = {}", Fraction::from_str("inf").unwrap());
    println!("Fraction::from_str(\"-inf\") = {}", Fraction::from_str("-inf").unwrap());
    println!("Fraction::from_str(\"nan\") = {}", Fraction::from_str("nan").unwrap());
}