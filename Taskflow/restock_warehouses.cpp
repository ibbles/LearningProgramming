// Project includes.
#include "utils.h"

// Taskflow includes.
#include "taskflow/taskflow.hpp"

// Standard library includes.
#include <iostream>

void start_orders() { std::cout << "Start Orders\n"; }
void check_main_warehouse() { std::cout << "Check Main Warehouse\n"; }
void prepare_main_warehouse_order() { std::cout << "Prepare Main Warehouse Order\n"; }
void unlock_backup_warehouse() { std::cout << "Unlock Backup Warehouse\n"; }
int condition() { std::cout << "Check Backup Warehouse\n"; return 0; }
void prepare_backup_warehouse_order() { std::cout << "Prepare Backup Warehouse Order\n"; }
void lock_backup_warehouse() { std::cout << "Lock Backup Warehosue\n"; }
void submit_orders() { std::cout << "Submit Orders\n"; }

int main()
{
	tf::Taskflow taskflow;
	tf::Executor executor;

	// Shared setup.
	tf::Task start_orders = taskflow.emplace(::start_orders);

	// Main warehouse tasks.
	tf::Task check_main_warehouse = taskflow.emplace(::check_main_warehouse);
	tf::Task prepare_main_warehouse_order = taskflow.emplace(::prepare_main_warehouse_order);

	// Backup warehouse tasks.
	tf::Task unlock_backup_warehouse = taskflow.emplace(::unlock_backup_warehouse);
	tf::Task check_backup_warehouse = taskflow.emplace(::condition);
	tf::Task prepare_backup_warehouse_order = taskflow.emplace(::prepare_backup_warehouse_order);
	tf::Task lock_backup_warehouse = taskflow.emplace(::lock_backup_warehouse);

	// Shared teardown.
	tf::Task submit_orders = taskflow.emplace(::submit_orders);

	// Main warehouse dependencies.
	start_orders.precede(check_main_warehouse);
	check_main_warehouse.precede(prepare_main_warehouse_order);
	prepare_main_warehouse_order.precede(submit_orders);

	// Backup warehouse dependencies.
	start_orders.precede(unlock_backup_warehouse);
	unlock_backup_warehouse.precede(check_backup_warehouse);
	check_backup_warehouse.precede(lock_backup_warehouse, prepare_backup_warehouse_order);
	prepare_backup_warehouse_order.precede(submit_orders);


	taskflow.name("Restock Warehouses");
	start_orders.name("Start Orders");
	check_main_warehouse.name("Check Main Warehouse");
	prepare_main_warehouse_order.name("Prepare Main Warehouse Order");
	unlock_backup_warehouse.name("Unlock Backup Warehouse");
	check_backup_warehouse.name("Check Backup Warehouse");
	prepare_backup_warehouse_order.name("Prepare Backup Warehouse Order");
	lock_backup_warehouse.name("Lock Backup Warehouse");
	submit_orders.name("Submit Orders");
	dumpToFile(taskflow, "restock_warehouses.dot");

	executor.run(taskflow).wait();
}
