### Visualizing simulation from a polytope with different random walks

library(ggplot2)
library(volesti)
for (step in c(100)){
  for (walk in c("CDHR", "RDHR", "BaW")){
    P <- gen_cube(100, 'H')
    points1 <- sample_points(P, n=1000, random_walk = list("walk" = walk, "walk_length" = step))
    g<-plot(ggplot(data.frame( x=points1[1,], y=points1[2,] )) + 
        geom_point( aes(x=x, y=y, color=walk)) + coord_fixed(xlim = c(-1,1), 
        ylim = c(-1,1)) + ggtitle(sprintf("walk length=%s", step, walk)))
  }
}