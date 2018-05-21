package assign10;

import java.util.PriorityQueue;
import java.util.ArrayList;

public class Assign10 {

    public PriorityQueue<Edge> edgeSet = new PriorityQueue<>();     //for sorting edges by weight
    public ArrayList<Edge> edgeMST = new ArrayList<>();             //for storing answer
    public int[] parentStr;                                         //for checking for transitive closure
    
    public static void main(String[] args) {
      int graph[][] = new int[][] {   { 0, 2, 4, 1, 0, 0, 0 },
                                      { 2, 0, 0, 3,10, 0, 0 },
                                      { 4, 0, 0, 2, 0, 5, 0 },
                                      { 1, 3, 2, 0, 7, 8, 4 },
                                      { 0,10, 0, 7, 0, 0, 6 },
                                      { 0, 0, 5, 8, 0, 0, 1 },
                                      { 0, 0, 0, 4, 6, 1, 0 } };
      
      Assign10 mainMethod = new Assign10();

      System.out.println("Reading graph...");		

      for(int i=0;i<graph.length;i++)
      {
        for(int j=0;j<graph[i].length;j++)
        {
          if(i<=j && graph[i][j]!=0)            //reading only half of the array
          {
            System.out.println("Added edge from " + (char)(i+65) + " to " + (char)(j+65) + " with dist " + graph[i][j]);
            Edge current = new Edge(i,j,graph[i][j]);       //create a new object
            mainMethod.edgeSet.add(current);                //put in priority queue
          }
        }
      }
      System.out.println("Added " + mainMethod.edgeSet.size() + " edges.");

      System.out.println("Computing MST...");
      mainMethod.kruskalMST(graph.length);                  //run Kruskal's algorithm
      
      int i=0;
      for(Edge e: mainMethod.edgeMST)
      {
     	i++;
        System.out.println("Edge " + i + ": from " + (char)(e.nodeA+65) + " to " + (char)(e.nodeB+65) + " with dist " + e.dist);        //print answers
      }
    }
  
    private void kruskalMST(int graphSize)
    {
      int edgeCount = 0;
      
      parentStr = new int[graphSize];           //init array
      for(int i=0;i<parentStr.length;i++)
        parentStr[i]=i;                         //init parent of nodes as itself
      
      while(edgeCount < graphSize - 1 && edgeSet.size() > 0)    //if not enough edges were selected and there's still edges left
      {
        Edge current = edgeSet.poll();          //get first edge from priority queue
        
        if(!isCycle(current.nodeA, current.nodeB))          //check if not a cycle
        {
         	edgeMST.add(current);                       //add to answer
          	graphJoining(current.nodeA, current.nodeB); //join two disjoint graph together
        }
      }
    }
  
    private boolean isCycle(int nodeA, int nodeB)
    {
      int parentA, parentB;

      parentA = findParent(nodeA);      //find parent for A
      parentB = findParent(nodeB);      //find parent for B

      return (parentA == parentB);            //return true if adding this edge will create a cycle, false otherwise
    }
  	
    private void graphJoining(int nodeA, int nodeB)
    {
      int parentA, parentB;

      parentA = findParent(nodeA);      //find parent for A
      parentB = findParent(nodeB);      //find parent for B
      
      parentStr[parentA] = parentB;     //join parents together
    }
  	
    private int findParent(int node)
    {
      int current = node;
      
      while(parentStr[current] != current)  //loop until parent of the node is the node itself
        current = parentStr[current];       //move to next node
      
      return current;       //return parent
    }
}
