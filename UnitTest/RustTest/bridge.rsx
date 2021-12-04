#[cxx::bridge]
mod add_fun {
    extern "Rust" {
        fn add(a:i32, b:i32) -> i32;
    }
}
