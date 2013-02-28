import System.Environment
import System.IO
import Control.Monad
import Data.List
import Data.Maybe

interactFile f fileName = do
  s <- readFile fileName
  putStr (f s)

-- | removeElements takes a two lists and removes all elements in the
-- second list that are also elements in the first list.
removeElements :: Eq a => [a] -> [a] -> [a]
removeElements xs ys = filter f ys
  where f y = null $ filter (y==) xs

expandIncludes :: String -> IO [String]
expandIncludes s =
  if isInclude s
     then
    readWithIncludes (includeFile s)
    else
    return [s]

readWithIncludes :: String -> IO [String]
readWithIncludes f = do
  s <- readFile f
  ss <- mapM expandIncludes (lines s)
  return (concat ss)

isInclude :: String -> Bool
isInclude s = isPrefixOf "#include" s

includeFile :: String -> String
includeFile s =  removeElements "<> \"" f
  where f = fromMaybe "" (stripPrefix "#include" s)

main = do
  args <- getArgs
  [res] <- mapM readWithIncludes args
  mapM putStrLn res


