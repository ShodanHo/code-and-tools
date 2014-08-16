int main()
{
  //...
  Terrain t(50);
  QuadTree qt(5);
  t.generate();
  qt.load(&t);
  //qt.parent should not be NULL anymore
  qt.createMesh(&mesh); //but apparently it is still NULL
  //...
}

Main(Line 3): qt.parent = NULL (from QuadTree.cpp line 8);
Main(Line 5): 
