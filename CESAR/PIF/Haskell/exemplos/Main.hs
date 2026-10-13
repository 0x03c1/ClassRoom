main :: IO ()
main = do
    putStrLn "Olá, Haskell!"
    putStrLn ("Soma: " ++ show (2 + 3 :: Int))
    putStrLn ("Dobro de 7: " ++ show (dobro 7))

dobro :: Int -> Int
dobro x = x * 2
