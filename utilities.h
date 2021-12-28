std::string int_to_string(const int);
void sort_tasks_by_decreasing_density(const TS&, unsigned short*);
bool test_schedulability(const int, const TS&);
void sort_tasks_by_DM(std::vector<unsigned short>&);
bool test_partition_schedulability(const TS&, std::vector<unsigned short>&);
