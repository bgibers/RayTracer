/* list.c */

#include "list.h"
#include <stdlib.h>
#include <stdio.h>

link_t::link_t(void)
{
	next = NULL;
	entity = NULL;
}

link_t::link_t(void *newentity)
{
	next = NULL;
	entity = newentity;

}

link_t::~link_t(void)
{
   fprintf(stderr, "in link destructor \n");
}

void link_t::set_next(link_t *new_next)
{
	next = new_next;
	
}

link_t * link_t::get_next()
{
	return next;


}

void * link_t::get_entity()
{
	return entity;
	
}


list_t::list_t()
{
	first = NULL;
	current = NULL;
	last = NULL;
}


void list_t::add(void *entity)
{

/* Create a new link passing it the entity pointer */
	link_t *newlink;
	newlink = new link_t(entity);

	



/* Now add the link to the list using generally the */
/* same approach as the C version                   */
	if(first == NULL){
		first = newlink;
	}
	else{
		last->set_next(newlink);
	}

	last = newlink;
	current = newlink;


}

void list_t::reset(void)
{
	current = first;

}

int list_t::not_end(void)
{
	return (current != NULL);

}

void list_t::next_link(void)
{
	assert(current != NULL);
	current = current->get_next();
}

void *list_t::get_entity(void)
{
	assert(current != NULL);
	return current->get_entity();

}

list_t::~list_t()
{
	link_t *kill;
	fprintf(stderr, "in list destructor \n");
	reset();
/* Delete each link_t in the list */
	while(not_end()){
		kill = current;
		next_link();
		delete kill;
	}

}

void list_t::insert(void *entity)
{

	if(first == NULL){
		add(entity);
	}
	else if(current == first){
		link_t *newlink =  new link_t(entity);
		newlink->set_next(first);
		first = newlink;
		current = newlink;
	}
	else if(current != first){
		link_t *traverse = new link_t;
		traverse = first;
		while(traverse != NULL && traverse->get_next() != current){
			traverse = traverse->get_next();
			
		}
		link_t *newlink = new link_t(entity);
		traverse->set_next(newlink);
		newlink->set_next(current);
		current = newlink;
	}
}

void list_t::remove(void)
{
	if(first == NULL){
		return;
	}
	else if(first == last && first == current){
		delete(current);
		first = NULL;
		current = NULL;
		last = NULL;
	}
	else if(current == first){
		first = current->get_next();
		delete(current);
		current = first;
	}
	else if(current == last){
		link_t *traverse = new link_t;
		traverse = first;
		while(traverse != NULL && traverse->get_next() != current){
			traverse = traverse->get_next();

		}

          last = traverse;
		last->set_next(NULL);

		delete(current);
		current = traverse;
	}
	else{
		 link_t *traverse = new link_t;
           traverse = first;
		 while(traverse != NULL && traverse->get_next() != current){
			traverse = traverse->get_next();
		 }
		 traverse->set_next(current->get_next());
		 delete(current);
		 current = traverse;
	}
}



