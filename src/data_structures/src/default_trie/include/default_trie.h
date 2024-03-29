#ifndef DEFAULT_TRIE_H
#define DEFAULT_TRIE_H

#include <unordered_map>
#include <vector>

#include "default_trie_node.h"

namespace ostp::libcc::data_structures {

/// Default trie data structure.
template <class K, class R>

/// A trie data structure that returns a default value for no matches.
class DefaultTrie {
   private:
    R default_return;               // Default return for no matches.
    std::vector<R> results;         // Vector of returns for each match in the trie.
    std::vector<int> free_slots;    // Vector of free slots in the results vector.
    std::vector<TrieNode<K>> trie;  // Trie data structure.
    int _size = 0;                  // Number of entries in the trie.

   public:
    /// Constructs a trie with the specified default return for no matches and for the root node.
    ///
    /// Arguments:
    ///     default_return: The default return for no matches.
    DefaultTrie(const R default_return) {
        // Add the root node to the trie.
        TrieNode<K> root;
        root.res = NO_MATCH;
        this->trie.push_back(root);
        this->default_return = default_return;
    }

    /// Returns the size of the trie.
    int size() { return this->_size; }

    /// Inserts the specfied entry to the trie with the specified return.
    ///
    /// Arguments:
    ///     entry: The entry to add to the trie.
    ///     entry_len: The length of the entry.
    ///     entry_return: The return for the entry.
    void insert(const K entry[], const int entry_len, const R entry_return) {
        // Traverse the trie until we reach the end of the entry or a node with no next entry.
        int node = 0;
        for (int i = 0; i < entry_len; i++) {
            // Create a new node if there is no next entry.
            if (trie[node].next.find(entry[i]) == trie[node].next.end()) {
                trie[node].next[entry[i]] = trie.size();
                TrieNode<K> new_node;
                new_node.res = NO_MATCH;
                trie.push_back(new_node);
            }

            // Move to the next node.
            node = trie[node].next[entry[i]];
        }

        // If there is already a return for the match ending in the last node, replace it otherwise
        // add it to the results vector.
        if (trie[node].res != NO_MATCH) {
            results[trie[node].res] = entry_return;
            return;
        } else {
            this->_size++;
            if (free_slots.size() > 0) {
                trie[node].res = free_slots.back();
                free_slots.pop_back();
                results[trie[node].res] = entry_return;
            } else {
                trie[node].res = results.size();
                results.push_back(entry_return);
            }
        }
    }

    /// Updates the default return for no matches.
    ///
    /// Arguments:
    ///     default_return: The new default return for no matches.
    void update_default_return(const R default_return) { this->default_return = default_return; }

    /// Removes the specified entry from the trie.
    ///
    /// Arguments:
    ///     entry: The entry to remove from the trie.
    ///     entry_len: The length of the entry.
    void remove(const K entry[], const int entry_len) {
        // Traverse the trie until we reach the end of the entry or a node with no next entry.
        int node = 0;
        for (int i = 0; i < entry_len; i++) {
            // Return if there is no next entry.
            if (trie[node].next.find(entry[i]) == trie[node].next.end()) {
                return;
            }

            // Move to the next node.
            node = trie[node].next[entry[i]];
        }

        // If there is a return for the match ending in the last node, remove it.
        if (trie[node].res != NO_MATCH) {
            this->_size--;
            results[trie[node].res] = NO_MATCH;
            free_slots.push_back(trie[node].res);
        }
    }

    /// Returns the return for the longest match in the trie for the specified entry.
    ///
    /// Arguments:
    ///     entry: The entry to get the return for.
    ///     entry_len: The length of the entry.
    R get(const K entry[], const int entry_len) {
        // Traverse the trie until we reach the end of the entry or a node with no next entry.
        int node = 0;
        for (int i = 0; i < entry_len; i++) {
            // Return the default return if there is no next entry.
            if (trie[node].next.find(entry[i]) == trie[node].next.end()) {
                return default_return;
            }

            // Move to the next node.
            node = trie[node].next[entry[i]];
        }

        // Return the result for the match ending in the last node or the default return if there
        // isn't one.
        if (trie[node].res == NO_MATCH) {
            return default_return;
        } else {
            return results[trie[node].res];
        }
    }

    /// Returns whether the trie contains the specified entry.
    ///
    /// Arguments:
    ///     entry: The entry to check for.
    ///     entry_len: The length of the entry.
    bool contains(const K entry[], const int entry_len) {
        // Traverse the trie until we reach the end of the entry or a node with no next entry.
        int node = 0;
        for (int i = 0; i < entry_len; i++) {
            // Return false if there is no next entry.
            if (trie[node].next.find(entry[i]) == trie[node].next.end()) {
                return false;
            }

            // Move to the next node.
            node = trie[node].next[entry[i]];
        }

        // Return whether there is a return for the match ending in the last node.
        return trie[node].res != NO_MATCH;
    }
};

}  // namespace ostp::libcc::data_structures

#endif
