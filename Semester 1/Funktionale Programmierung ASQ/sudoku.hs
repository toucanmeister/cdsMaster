
{-# Language GeneralizedNewtypeDeriving #-}
{-# Language ScopedTypeVariables #-}
{-# Language InstanceSigs #-}
{-# Language FlexibleContexts #-}
{-# Language UndecidableInstances #-}

import Data.List

type Matrix a = [Row a]
type Row a = [a]
type Grid = Matrix Digit
type Digit = Char
type Choices = [Digit]

digits :: [Char]
digits = ['1'..'9']

blank :: Char -> Bool
blank = (=='0')

solve :: Grid -> [Grid]
solve = filter valid . expand . choices

choices :: Grid -> Matrix Choices
choices = map (map choice)

choice :: Char -> [Char]
choice d = if blank d then digits else [d]

expand :: Matrix Choices -> [Grid]
expand = cartProd . map cartProd

cartProd :: [[a]] -> [[a]]
cartProd [] = [[]]
cartProd (xs:xss) = [x:ys | x <- xs, ys <- cartProd xss]

valid :: Grid -> Bool
valid g = all nodups (rows g) && all nodups (cols g) && all nodups (boxs g)

nodups :: Eq a => [a] -> Bool
nodups [] = True
nodups (x:xs) = notElem x xs && nodups xs

rows :: Matrix a -> Matrix a
rows = id

cols :: Matrix a -> Matrix a
cols = transpose

boxs :: Eq a => Matrix a -> Matrix a
boxs = map ungroup . ungroup . map cols . group . map group

ungroup :: [[a]] -> [a]
ungroup = concat 

prune :: Matrix Choices -> Matrix Choices
prune = pruneBy boxs . pruneBy cols . pruneBy rows

pruneBy :: ([Row Choices] -> [Row Choices]) -> [Row Choices] -> [Row Choices]
pruneBy f = f . map pruneRow . f

pruneRow :: Row Choices -> Row Choices
pruneRow row = map (remove fixed) row
  where fixed = [d | [d] <- row]

remove xs ds = if length ds == 1 then xs else ds

solveP :: Grid -> [Grid]
solveP = filter valid . expand . prune . choices
