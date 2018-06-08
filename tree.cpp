template <typename Comparable>
class BinarySearchTree
{
public:
  BinarySearchTree();
  BinarySearchTree( const BinarySearchTree & rhs );
  ~BinarySearchTree();

  const Comparable & findMin() const;
  const Comparable & findMax() const;
  bool contains( const Comparable & x ) const;
  bool isEmpty() const;
  void printTree() const;

  void makeEmpty();
  void insert( const Comparable & x );
  void remove( const Comparable & x );

  const BinarySearchTree & operator=( const BinarySearchTree & rhs );           //

private:
  struct BinaryNode
  {
    Comparable element;
    BinaryNode *left;
    BinaryNode *right;

    BinaryNode( const Comparable & theElement, BinaryNode *lt, BinaryNode *rt)
      : element( theElement ), left( lt ), right( rt ){ }
  };

  BinaryNode *root;

  void insert( const Comparable & x, BinaryNode * & t ) const;                  //use the technique of passing a pointer variable using call by reference. This allows
  void remove( const Comparable & x, BinaryNode * & t ) const;                  //the public member functions to pass a pointer to the root to the private recursive
  BinaryNode * findMin( BinaryNode *t ) const;                                  //member functions. The recursive functions can then change the value of the root so
  BinaryNode * findMax( BinaryNode *t ) const;                                  //that the root points to another node.
  bool contains( const Comparable & x, BinaryNode *t ) const;
  void makeEmpty( BinaryNode * & t );
  void printTree( BinaryNode *t ) const;
  BinaryNode * clone( BinaryNode *t ) const;
}

bool contains( const Comparable & x ) const
{
  return contains(x, root);
}

void insert( const Comparable & x )
{
  insert( x, root );
}

void remove( const Comparable & x )
{
  remove( x, root );
}

bool contains( const Comparable & x, BinaryNode * t) const
{
  if( t == NULL )
    return false;
  else if( x < t->element)
    return contains( x, t->left );
  else if( t->element < x )
    return contains( x, t->right );
  else
    return true;
}

BinaryNode * findMin( BinaryNode *t ) const
{
  if( t == NULL )
    return NULL;
  if( t->left == NULL )
    return t;
  return findMin( t-> left );
}

BinaryNode * findMax( BinaryNode * t ) const
{
  if( t != NULL)
    while( t->right != NULL )
      t = t->right
  return t;
}

void insert( const Comparable & x, BinaryNode * & t )
{
  if( t == NULL )
    t = new BinaryNode( x, NULL, NULL );
  else if( x < t->element)
    insert( x, t->left );
  else if( t->element < x )
    insert( x, t->right );
  else
    ;       // Duplicate; do nothing
}

/**
 * Internal method to remove from a subtree.
 * x is the item to remove.
 * t is the node that roots the subtree.
 * Set the new root of the subtree.
 */
void remove( const Comparable & x, BinaryNode * & t )
{
  if( t == NULL )
    return; // Item not found; do nothing
  if( x < t->element )
    remove( x, t->left );
  else if( t->element < x )
    remove( x, t->right );
  else if( t->left != NULL && t->right != NULL) // Two children
  {
    /**
     * If the node has two children, the general strategy is to replace the data of this node with the smallest data of the right subtree(which is easily found)
     * and recursively delete that node (which is now empty).
     *
     * This is inefficient, because it makes two passes down the tree to find and delete the smallest node in the right subtree when this is appropriate.
     * It is easily to remove this inefficiency, by writing a special removeMin method.
     */
    t->element = findMin( t-> right )->element;
    remove( t->element, t->right );
  }
  else
  {
    /**
     * If the node is a leaf, it can be delete immediately.
     * If the node has one child, the node can be delete after its parent adjusts a link to bypass the node.
     */
    BinaryNode * oldNode = t;
    t = ( t->left != NULL ) ? t->left : t->right;
    delete oldNode;
  }
}

/**
 * Destructor for the tree
 */
~BinarySearchTree()
{
  makeEmpty();
}

 /**
  * Internal method to make subtree empty.
  */
void makeEmpty(BinaryNode * & t)
{
  if( t != NULL )
  {
    /*
     * after recursively processing t's children, a call to delete is made for t.
     * Thus all nodes are recursively reclaimed.
     */
    makeEmpty( t->left );
    makeEmpty( t->right );
    delete t;
  }
  /**
   * Notice that at the end, t, and thus root, is changed to point at NULL.
   */
  t = NULL;
}

/*
 * Deep copy.
 */
 const BinarySearchTree & operator=( const BinarySearchTree & rhs )
 {
   if( this != &rhs )
   {
     makeEmpty();
     root = clone( rhs.root );
   }
   return *this;
 }

 /*
  * Internal method to clone subtree.
  */
BinaryNode * clone( BinaryNode *t ) const
{
  if( t == NULL )
    return NULL;

  return new BinaryNode( t->element, clone( t->left ), clone( t->right ) );
}


//Illustrates use of a function object to implement binary search tree
template <typename Object, typename Comparator=less<Object> >
class BinarySearchTree
{
public:
  //same method, with Object replacing Comparable
  BinarySearchTree();
  BinarySearchTree( const BinarySearchTree & rhs );
  ~BinarySearchTree();

  const Object & findMin() const;
  const Object & findMax() const;
  bool contains( const Object & x ) const;
  bool isEmpty() const;
  void printTree() const;

  void makeEmpty();
  void insert( const Object & x );
  void remove( const Object & x );

  const BinarySearchTree & operator=( const BinarySearchTree & rhs );           //

private:

  struct BinaryNode
  {
    Object element;
    BinaryNode *left;
    BinaryNode *right;

    BinaryNode( const Object & theElement, BinaryNode *lt, BinaryNode *rt)
      : element( theElement ), left( lt ), right( rt ){ }
  };

  BinaryNode *root;
  Comparator isLessThan;
  //Same method, with Object replacing Comparable

  void insert( const Object & x, BinaryNode * & t ) const;                      // use the technique of passing a pointer variable using call by reference. This allows
  void remove( const Object & x, BinaryNode * & t ) const;                      // the public member functions to pass a pointer to the root to the private recursive
  BinaryNode * findMin( BinaryNode *t ) const;                                  // member functions. The recursive functions can then change the value of the root so
  BinaryNode * findMax( BinaryNode *t ) const;                                  // that the root points to another node.

  /**
   * Internal method to test if an item is in a subtree.
   * x is item to search for.
   * t is the node that roots the subtree.
   */
  bool contains( const Object & x, BinaryNode *t ) const
  {
    if( t == NULL )
      return false;
    else if( isLessThan( x, t->element ) )
      return contains( x, t->left );
    else if( isLessThan( t->element, x ) )
      return contains( x, t->right );
    else
      return true; // Match
  }
  void makeEmpty( BinaryNode * & t );
  void printTree( BinaryNode *t ) const;
  BinaryNode * clone( BinaryNode *t ) const;
}
