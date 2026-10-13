import Text.Read (readMaybe)

lerInteiro :: String -> Maybe Int
lerInteiro texto = do
    n <- readMaybe texto :: Maybe Integer
    if n < toInteger (minBound :: Int) || n > toInteger (maxBound :: Int)
        then Nothing
        else Just (fromInteger n)

main :: IO ()
main = do
    putStrLn "Digite um número inteiro:"
    entrada <- getLine
    case lerInteiro entrada of
        Nothing -> putStrLn "Entrada inválida."
        Just n  -> putStrLn ("Dobro: " ++ show (n * 2))
