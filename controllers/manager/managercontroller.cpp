#include "managercontroller.h"

ManagerController::ManagerController(const User &managerUser, AuthModel *authModel, QObject *parent)
    : QObject(parent)
    , m_view(nullptr)
    , m_managerModel(new ManagerModel(this)) {

    m_view = new ManagerWindow(managerUser, m_managerModel);
    connect(m_view, &ManagerWindow::logoutRequested,
            this, &ManagerController::handleLogout);
}

ManagerController::~ManagerController() {
    if (m_view) {
        m_view->deleteLater();
    }
}

void ManagerController::start() {
    m_view->show();
}

void ManagerController::handleLogout() {
    emit logout();
}