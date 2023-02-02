
{-# Language GeneralizedNewtypeDeriving #-}
{-# Language ScopedTypeVariables #-}
{-# Language InstanceSigs #-}
{-# Language FlexibleContexts #-}
{-# Language UndecidableInstances #-}

fib :: (Int -> Int) -> Int -> Int
fib f 0 = 1
fib f 1 = 1
fib f n = f (n-1) + f (n-2)

fix :: (f -> f) -> f
fix f = let x = f x in x

memoList :: [Int] -> (Int -> a) -> (Int -> a)
memoList ks f = (map f ks !!)