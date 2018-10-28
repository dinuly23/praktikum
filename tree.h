#pragma once

#include <memory>
#include <iostream>

namespace bintree {
    template <typename T>
    //отнаследовались от std::enable_shared_from_this для устрания проблемы 
    //с попыткой создания shared_ptr из метода самого объекта
    struct TNode : std::enable_shared_from_this <TNode<T>> {		
        using TNodePtr = std::shared_ptr<TNode<T>>;
        using TNodeConstPtr = std::shared_ptr<const TNode<T>>;
        using TNodeWeakPtr = std::weak_ptr<TNode<T>>;	    //для странения перекрестных ссылок на parent указываем weak_ptr 

        bool hasLeft() const {
            return bool(left);
        }

        bool hasRight() const {
            return bool(right);
        }

        bool hasParent() const {
            return bool(parent);
        }

        T& getValue() {
            return value;
        }

        const T& getValue() const {
            return value;
        }

        TNodePtr getLeft() {
            return left;
        }

        TNodeConstPtr getLeft() const {
            return left;
        }

        TNodePtr getRight() {
            return right;
        }

        TNodeConstPtr getRight() const {
            return right;
        }

        TNodePtr getParent() {
	    if (parent.expired())				// проверяем удален ли родитель
            	return std::shared_ptr<TNode>(nullptr);		// возвращаем shared_ptr на null
	    else						// иначе, если не удален родитель 
            	return std::shared_ptr<TNode>(parent);		// возвращаем shared_ptr на родителя  
        }

        TNodeConstPtr getParent() const {
            return parent;
        }

        static TNodePtr createLeaf(T v) {
	    struct B:public TNode{	// создаем структуру для обхода Private конструктора TNode
		B(T v) : TNode(v) {}	// создаем конструктор, вызывающий конструктор TNode
	    }; 				
            return std::make_shared<B>(v);
        }

        static TNodePtr fork(T v, TNode* left, TNode* right) {
	    struct B:public TNode{						// создаем структуру для обхода Private конструктора TNode
		B(T v, TNode* left, TNode* right) : TNode(v,left,right) {}	// создаем конструктор, вызывающий конструктор TNode
	    }; 									
            TNodePtr ptr = std::make_shared<B>(v, left, right);
            setParent(ptr->getLeft(), ptr);
            setParent(ptr->getRight(), ptr);
            return ptr;
        }

        TNodePtr replaceLeft(TNodePtr l) {
            setParent(l, std::enable_shared_from_this <TNode<T>>::shared_from_this());	// используем shared_from_this() для устранения проблемы с shard_ptr(this)
            setParent(left, nullptr);
            std::swap(l, left);
            return l;
        }

        TNodePtr replaceRight(TNodePtr r) {
            setParent(r, std::enable_shared_from_this <TNode<T>>::shared_from_this());	// используем shared_from_this() для устранения проблемы с shard_ptr(this)
            setParent(right, nullptr);
            std::swap(r, right);
            return r;
        }

        TNodePtr replaceRightWithLeaf(T v) {
            return replaceRight(createLeaf(v));
        }

        TNodePtr replaceLeftWithLeaf(T v) {
            return replaceLeft(createLeaf(v));
        }

        TNodePtr removeLeft() {
            return replaceLeft(nullptr);
        }
        TNodePtr removeRight() {
            return replaceRight(nullptr);
        }

    private:
        T value;
        TNodePtr left = nullptr;
        TNodePtr right = nullptr;
        TNodeWeakPtr parent = TNodePtr(nullptr);  // указываем на parent weak_ptr  

        TNode(T v)
            : value(v)
        {
        }
        TNode(T _v, TNode* _left, TNode* _right)
            : value(_v)
        {
		class deleter						// создаем свой удалитель для shared_ptr 
		{							// для устранения повторного удаления объекта 
		public:							
			void operator()(TNode * p) {}			// удалитель ничего не делает, при fork ничего не требуется делать с объектом  
		};							
		left = std::shared_ptr<TNode<T> >(_left,deleter());	// создаем shared_ptr со своим удалителем 
		right = std::shared_ptr<TNode<T> >(_right,deleter());	// создаем shared_ptr со своим удалителем
        }

        static void setParent(TNodePtr node, TNodePtr parent) {
            if (node) {
		if (parent)					
                	node->parent = parent;
		else
			node->parent = TNodePtr(nullptr);		// обработка случая при нулевом родителе
	    }
        }
    };
}
