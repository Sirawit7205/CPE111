package assign10;

public class Edge implements Comparable<Edge>{
    int nodeA, nodeB, dist;
    
    public Edge() {}
    
    public Edge(int nodea, int nodeb, int distance)
    {
        nodeA = nodea;          //first node
        nodeB = nodeb;          //second node
        dist = distance;        //weight
    }
    
    @Override
    public int compareTo(Edge o)
    {
        if(this.dist != o.dist)
          return (int) this.dist - o.dist;      //sort by weight first
      	else if(this.nodeA != o.nodeA)
          return (int) this.nodeA - o.nodeA;    //then by first node
        else
          return (int) this.nodeB - o.nodeB;    //then by second node
    }
}
