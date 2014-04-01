-- One Dimensional Cellular Automation

import qualified System.Environment as Env
import qualified System.Random as Rand

type Generation = [Int]

main :: IO [()]
main = Env.getArgs >>= start >>= sequence . (map putGen)

start :: [String] -> IO [Generation]
start args
  | length args < 2 = fail "Too few arguments"
  | otherwise = initGen (read $ args !! 0)
                >>= (\x -> return $ run x (read $ args !! 1))
    
putGen :: [Int] -> IO ()
putGen g = (sequence $ map (putStr . show) g) >> putStrLn ""

run :: Generation -> Int -> [Generation]
run _ 0 = []
run g n = g:(run (update g) (n-1))

update :: Generation -> Generation
update g = map abs $ zipWith (-) lefts rights
  where lefts = 0:(init g)
        rights = (tail g) ++ [0]

initGen :: Int -> IO Generation
initGen n = Rand.getStdGen >>= return . randList n

randList :: Rand.RandomGen a => Int -> a -> [Int]
randList 0 _ = []
randList n r = (fst new):(randList (n-1) (snd new))
  where new = Rand.randomR (0,1) r
