#include <set>
#include "QueryPlan.hpp"

// The Plan Tree Node constructor
PlanTreeNode::PlanTreeNode() {}

// The Plan Tree constructor
PlanTree::PlanTree(std::set<table_t*>& tableSet) {}

// Merges two join trees into one
// and return the resulted tree
PlanTree* PlanTree::mergePlanTrees(PlanTree* left, PlanTree* right) {
    return NULL;
}

// execute the plan described by a Plan Tree
void executePlan(PlanTree* planTreePtr) {}

// Destoys a Plan Tree properly
void PlanTree::freePlanTree(PlanTree* planTreePtr) {}

// Prints a Plan Tree -- for debugging
void PlanTree::printPlanTree(PlanTree* planTreePtr) {}

// Estimates the cost of a given Plan Tree */
double PlanTree::costOfPlanTree(PlanTree* planTreePtr) {
    return 1.0;
}

// Builds a query plan with the given info
void QueryPlan::build(QueryInfo& queryInfoPtr) {
#ifdef k
    /* a map that maps each relation-ID to a pointer to it's respective table-type */
    std::map<RelationId, table_t*> tableTPtrMap;

    /* apply all filters and create a vector of table-types from the query selections */
    for (std::vector<FilterInfo>::iterator it = queryInfoPtr.filters.begin(); it != queryInfoPtr.filters.end(); ++it) {
        std::map<RelationId, table_t*>::iterator mapIt = tableTPtrMap.find(it->filterColumn.relId);

        /* if table-type for this relation-ID already in map, then update the respective table-type */
        if (mapIt != tableTPtrMap.end()) {
            Select(it, mapIt->second);
        }
        else {
            /* otherwise add the table-type's mapping to the map */
            table_t* tempTableTPtr = CreateTableTFromId(it->filterColumn.relId, it->filterColumn.binding);
            Select(it, tempTableTPtr);
            tableTPtrMap.insert(it->filterColumn.relId, tempTableTPtr);
        }
    }

    /* create a set of pointers to the table-types that are to be joined */
    std::set<table_t*> tableTPtrSet;
    for (std::vector<PredicateInfo>::iterator it = queryInfoPtr.predicates.begin(); it != queryInfoPtr.predicates.end(); ++it) {
        std::map<RelationId, table_t*>::iterator mapItLeft = tableTPtrMap.find(it->left.relId),
                                            mapItRight = tableTPtrMap.find(it->right.relId);
        /* sanity check -- REMOVE in the end */
        assert(mapItLeft != tableTPtrMap.end() && mapItRight != tableTPtrMap.end());

        tableTPtrSet.insert(mapItLeft);
        tableTPtrSet.insert(mapItRight);
    }

    /* create a Join Tree by joining all the table-types of the set */
    JoinTree* tempJoinTreePtr = constrJoinTreeFromRelations(tableTPtrSet);

    /* apply all filters */
    for (std::vector<int>::iterator it = queryInfoPtr.filters.begin() ; it != queryInfoPtr.filters.end(); ++it) {}

    /* create an array of sets of relations to be joined */

    /* create an array of Join Trees for each set of relations */

    /* merge all Join Trees to one final Join Tree */
#endif
}

// Fills the columnInfo matrix with the data of every column
void QueryPlan::fillColumnInfo(Joiner& joiner) {
    Relation* relation;
    int relationsCount = joiner.getRelationsCount(); // Get the number of relations
    int columnsCount; // Number of columns of a relation

    // Allocate memory for every relation
    columnInfos = (ColumnInfo**) malloc(relationsCount * sizeof(ColumnInfo*));

    // For every relation get its column statistics
    for (int rel = 0; rel < relationsCount; rel++) {
        // Get the number of columns
        relation = &(joiner.getRelation(rel));
        columnsCount = relation->columns.size();

        // Allocate memory for the columns
        columnInfos[rel] = (ColumnInfo*) malloc(columnsCount * sizeof(ColumnInfo));

        std::cerr << "relation: " << rel << std::endl;
        flush(std::cerr);

        // Get the info of every column
        for (int col = 0; col < columnsCount; col++) {
            uint64_t minimum = std::numeric_limits<uint64_t>::max(); // Value of minimum element
            uint64_t maximum = 0; // Value of maximum element
            std::set<uint64_t> distinctElements; // Keep the distinct elements of the column

            uint64_t tuples = relation->size;
            for (int i = 0; i < tuples; i++) {
                uint64_t element = relation->columns[col][i];
                if (element > maximum) maximum = element;
                if (element < minimum) minimum = element;
                distinctElements.insert(element);
            }

            // Save the infos
            columnInfos[rel][col].min = minimum;
            columnInfos[rel][col].max = maximum;
            columnInfos[rel][col].size = tuples;
            columnInfos[rel][col].distinct = (uint64_t) distinctElements.size();

            std::cerr << "    column: " << col << std::endl;
            std::cerr << "    min: " << columnInfos[rel][col].min << std::endl;
            std::cerr << "    max: " << columnInfos[rel][col].max << std::endl;
            std::cerr << "    size: " << columnInfos[rel][col].size << std::endl;
            std::cerr << "    distinct: " << columnInfos[rel][col].distinct << std::endl << std::endl;
            flush(std::cerr);
        }
    }
}
