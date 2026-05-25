#pragma once
#include <cstdlib>
#include <iostream>
#include <functional>
#include <tuple>
#include <any>

/// <param name="func">Function tree is created for</param>
template<typename ReturnType, typename... InputTypes>
TreeList<ReturnType, InputTypes...>::TreeList(ReturnType(*func)(InputTypes...))
	: func_(func) {
	CreateTrees();
}

template<typename ReturnType, typename... InputTypes>
void TreeList<ReturnType, InputTypes...>::CreateTrees() {
	trees = std::make_tuple(Tree<InputTypes>()...);

	[&] <size_t... Is>(std::index_sequence<Is...>) {
		(CreateTreeList<Is>(), ...);
	}(std::index_sequence_for<InputTypes...>{});

}

/// <summary>
/// Fills VariantList (LLT)
/// </summary>
template<typename ReturnType, typename... InputTypes>
template<size_t idx>
void TreeList<ReturnType, InputTypes...>::CreateTreeList() {
	treelist.push_back(PossibleTypes{ std::in_place_index<idx>, &std::get<idx>(trees) });
}

template<typename ReturnType, typename... InputTypes>
template<size_t idx>
void TreeList<ReturnType, InputTypes...>::SetValues(const std::tuple<InputTypes...>& data) {
	std::get<idx>(trees).SetSearchValue(std::get<idx>(data));
}

template<typename ReturnType, typename... InputTypes>
ReturnType TreeList<ReturnType, InputTypes...>::GetValue(InputTypes... params) {

	paramsTuple = std::make_tuple(params...);

	// updating searchValue in trees according to passed parameters
	[&] <size_t... Is>(std::index_sequence<Is...>) {
		((Is < paramsCount ?
			SetValues<Is>(paramsTuple) : void()), ...);
	}(std::index_sequence_for<InputTypes...>{});

	auto* it = treelist.head.get(); // iterator

	void* currHead = static_cast<void*>(&head->next); // remembering the head of the first tree
	void* currNode = static_cast<void*>(head->next); // leaf to search from

	// searching in every tree
	while (it) {
		// searching in tree on some level, FindAbstract returns a pointer to the leaf on a NEXT level (head of next tree)
		currNode = std::visit([&](auto& tree) -> void* {
			return tree->FindAbstract(currNode);
			}, it->tree);

		if (currNode == nullptr) break; // node is not found on one of the trees, need to create

		if (!it->next.get()) break; // node is found and this is the last tree (the next tree doesn't exist)

		// found node functions as a head of the tree on a next level
		currHead = currNode;
		currNode = *static_cast<void**>(currNode);
		it = it->next.get(); // moving to the next tree
	}

	void* val = nullptr;
	if (currNode == nullptr)
	{ // node is not found.
	  // the iterator is still on a tree that misses the element. 
	  // so we need to create leaf on this tree and next trees and connect them.
		//std::cout << "CAHCHE MISS, CALLING FUNCTION\n";
		currNode = std::visit([&](auto& tree) -> void* {
			void* tmp = tree->InsertNodeAbstract(currHead);
			void* updatedNode = *static_cast<void**>(currHead);
			return tmp;
			}, it->tree); // found a place for a new node, created it, returned node->next (head of a next tree that is yet nullptr)

		while (it->next.get()) {
			it = it->next.get(); // moving to the next tree

			currNode = std::visit([&](auto& tree) -> void* {
				return tree->NewNodeAbstract(currNode);
				}, it->tree);
		}

		// calling a function 
		auto* tmpRes = new ReturnType(std::apply(func_, paramsTuple));

		// writing a result of function to the next parameter of last leaf 
		*static_cast<void**>(currNode) = static_cast<void*>(tmpRes);

		val = tmpRes;
	}
	else if (!it->next.get()) { // node found and this is the last tree
		//std::cout << "FOUND\n";
		// returning currNode 
		val = *static_cast<void**>(currNode);
	}

	if (val == nullptr) {
		throw std::runtime_error("Critial: val is nullptr");
	}

	ReturnType* typedVal = static_cast<ReturnType*>(val);
	return *typedVal;
}