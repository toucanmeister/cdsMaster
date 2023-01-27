import Data.Foldable
import Data.Ord


data Tree = Leaf Int | Fork Tree Tree -- min height trees
type Forest = [Tree]

cost :: Tree -> Int
cost (Leaf x) = x
cost (Fork l r) = 1 + max (cost l) (cost r)

minCost :: Forest -> Tree
minCost = minimumBy (comparing cost)

trees :: [Int] -> Forest
trees [x] = [Leaf x]
trees (x:xs) = concatMap (prefixes x) (trees xs)

prefixes :: Int -> Tree -> [Tree]
prefixes x t@(Leaf y) = [Fork (Leaf x) t]
prefixes x t@(Fork l r) = Fork (Leaf x) t : [Fork l' r | l' <- prefixes x l]

mincostTree :: [Int] -> Tree
mincostTree = minimumBy (comparing cost) . trees