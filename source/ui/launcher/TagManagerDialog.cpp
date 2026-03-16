#include "TagManagerDialog.h"
#include "../ThemeColors.h"
#include "../../core/NotebookLibrary.h"
#include "../../core/Document.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>
#include <QInputDialog>
#include <QMessageBox>
#include <QScrollBar>

TagManagerDialog::TagManagerDialog(const QString& bundlePath, QWidget* parent)
    : QDialog(parent)
    , m_bundlePath(bundlePath)
{
    setWindowTitle(tr("Manage Tags"));
    setModal(true);
    setMinimumSize(DIALOG_MIN_WIDTH, DIALOG_MIN_HEIGHT);

    // Load current tags from the notebook if provided
    if (!m_bundlePath.isEmpty()) {
        QString docPath = m_bundlePath + "/document.json";
        QFile file(docPath);
        if (file.open(QIODevice::ReadOnly)) {
            QJsonParseError parseError;
            QJsonDocument doc = QJsonDocument::fromJson(file.readAll(), &parseError);
            if (parseError.error == QJsonParseError::NoError && doc.isObject()) {
                m_selectedTags = doc.object()["tags"].toVariant().toStringList();
            }
            file.close();
        }
    }

    setupUi();
    applyTheme();
    populateTags();
}

QStringList TagManagerDialog::getAllTagsFromLibrary() const
{
    QSet<QString> tagSet;
    const auto& notebooks = NotebookLibrary::instance()->notebooks();
    for (const auto& nb : notebooks) {
        for (const QString& tag : nb.tags) {
            tagSet.insert(tag);
        }
    }
    QStringList allTags = tagSet.toList();
    allTags.sort(Qt::CaseInsensitive);
    return allTags;
}

void TagManagerDialog::setupUi()
{
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(MARGIN, MARGIN, MARGIN, MARGIN);
    mainLayout->setSpacing(SPACING);

    // Title
    m_titleLabel = new QLabel(tr("Tags"));
    QFont titleFont = m_titleLabel->font();
    titleFont.setPointSize(16);
    titleFont.setBold(true);
    m_titleLabel->setFont(titleFont);
    mainLayout->addWidget(m_titleLabel);

    // Search input
    m_searchInput = new QLineEdit();
    m_searchInput->setPlaceholderText(tr("Search tags..."));
    m_searchInput->setMinimumHeight(SEARCH_HEIGHT);
    mainLayout->addWidget(m_searchInput);
    connect(m_searchInput, &QLineEdit::textChanged, this, &TagManagerDialog::onSearchTextChanged);

    // Horizontal layout for two lists
    QHBoxLayout* listsLayout = new QHBoxLayout();
    listsLayout->setSpacing(SPACING);

    // All tags list
    QVBoxLayout* allTagsLayout = new QVBoxLayout();
    QLabel* allTagsLabel = new QLabel(tr("Available Tags"));
    allTagsLabel->setFont(titleFont);
    allTagsLabel->setPointSize(12);
    allTagsLayout->addWidget(allTagsLabel);

    m_tagList = new QListWidget();
    m_tagList->setMinimumHeight(200);
    m_tagList->setSelectionMode(QAbstractItemView::SingleSelection);
    allTagsLayout->addWidget(m_tagList);
    connect(m_tagList, &QListWidget::itemClicked, this, &TagManagerDialog::onTagClicked);

    listsLayout->addLayout(allTagsLayout);

    // Selected tags list
    QVBoxLayout* selectedLayout = new QVBoxLayout();
    QLabel* selectedLabel = new QLabel(tr("Notebook Tags"));
    selectedLabel->setFont(titleFont);
    selectedLabel->setPointSize(12);
    selectedLayout->addWidget(selectedLabel);

    m_selectedTagList = new QListWidget();
    m_selectedTagList->setMinimumHeight(200);
    m_selectedTagList->setSelectionMode(QAbstractItemView::SingleSelection);
    selectedLayout->addWidget(m_selectedTagList);
    connect(m_selectedTagList, &QListWidget::itemClicked, this, &TagManagerDialog::onTagClicked);

    listsLayout->addLayout(selectedLayout);

    mainLayout->addLayout(listsLayout);

    // Button row
    QHBoxLayout* buttonLayout = new QHBoxLayout();
    buttonLayout->setSpacing(SPACING);

    m_addTagButton = new QPushButton(tr("Add Tag"));
    m_addTagButton->setMinimumHeight(BUTTON_HEIGHT);
    buttonLayout->addWidget(m_addTagButton);
    connect(m_addTagButton, &QPushButton::clicked, this, &TagManagerDialog::onAddTagClicked);

    m_deleteTagButton = new QPushButton(tr("Delete"));
    m_deleteTagButton->setMinimumHeight(BUTTON_HEIGHT);
    m_deleteTagButton->setEnabled(false);
    buttonLayout->addWidget(m_deleteTagButton);
    connect(m_deleteTagButton, &QPushButton::clicked, this, &TagManagerDialog::onDeleteTagClicked);

    buttonLayout->addStretch();

    m_cancelButton = new QPushButton(tr("Cancel"));
    m_cancelButton->setMinimumHeight(BUTTON_HEIGHT);
    buttonLayout->addWidget(m_cancelButton);
    connect(m_cancelButton, &QPushButton::clicked, this, &TagManagerDialog::onCancelClicked);

    m_applyButton = new QPushButton(tr("Apply"));
    m_applyButton->setMinimumHeight(BUTTON_HEIGHT);
    m_applyButton->setDefault(true);
    buttonLayout->addWidget(m_applyButton);
    connect(m_applyButton, &QPushButton::clicked, this, &TagManagerDialog::onApplyClicked);

    mainLayout->addLayout(buttonLayout);
}

void TagManagerDialog::applyTheme()
{
    if (m_darkMode) {
        setStyleSheet(QString(
            "QDialog { background-color: %1; color: %2; }"
            "QListWidget { background-color: %3; border: 1px solid %4; border-radius: 8px; }"
            "QListWidget::item { padding: 8px; border-radius: 4px; }"
            "QListWidget::item:selected { background-color: %5; }"
            "QListWidget::item:hover { background-color: %6; }"
            "QLineEdit { background-color: %3; border: 1px solid %4; border-radius: 8px; padding: 8px; color: %2; }"
            "QPushButton { background-color: %7; border: none; border-radius: 8px; padding: 8px 16px; color: white; font-weight: bold; }"
            "QPushButton:hover { background-color: %8; }"
            "QPushButton:pressed { background-color: %9; }"
            "QPushButton[accessibleName=\"secondary\"] { background-color: %3; border: 1px solid %4; color: %2; }"
        ).arg(
            ThemeColors::dialogBackground(true).name(),
            ThemeColors::textPrimary(true).name(),
            ThemeColors::inputBackground(true).name(),
            ThemeColors::inputBorder(true).name(),
            ThemeColors::selection(true).name(),
            ThemeColors::hover(true).name(),
            ThemeColors::accent(true).name(),
            ThemeColors::accentHover(true).name(),
            ThemeColors::accentPressed(true).name()
        ));
    } else {
        setStyleSheet(QString(
            "QDialog { background-color: %1; color: %2; }"
            "QListWidget { background-color: %3; border: 1px solid %4; border-radius: 8px; }"
            "QListWidget::item { padding: 8px; border-radius: 4px; }"
            "QListWidget::item:selected { background-color: %5; }"
            "QListWidget::item:hover { background-color: %6; }"
            "QLineEdit { background-color: %3; border: 1px solid %4; border-radius: 8px; padding: 8px; color: %2; }"
            "QPushButton { background-color: %7; border: none; border-radius: 8px; padding: 8px 16px; color: white; font-weight: bold; }"
            "QPushButton:hover { background-color: %8; }"
            "QPushButton:pressed { background-color: %9; }"
            "QPushButton[accessibleName=\"secondary\"] { background-color: %3; border: 1px solid %4; color: %2; }"
        ).arg(
            ThemeColors::dialogBackground(false).name(),
            ThemeColors::textPrimary(false).name(),
            ThemeColors::inputBackground(false).name(),
            ThemeColors::inputBorder(false).name(),
            ThemeColors::selection(false).name(),
            ThemeColors::hover(false).name(),
            ThemeColors::accent(false).name(),
            ThemeColors::accentHover(false).name(),
            ThemeColors::accentPressed(false).name()
        ));
    }
}

void TagManagerDialog::populateTags()
{
    m_allTags = getAllTagsFromLibrary();

    // Update both lists
    m_tagList->clear();
    m_selectedTagList->clear();

    for (const QString& tag : m_allTags) {
        if (!m_selectedTags.contains(tag)) {
            m_tagList->addItem(tag);
        }
    }

    for (const QString& tag : m_selectedTags) {
        m_selectedTagList->addItem(tag);
    }
}

void TagManagerDialog::filterTags(const QString& text)
{
    // Filter both lists
    for (int i = 0; i < m_tagList->count(); ++i) {
        QListWidgetItem* item = m_tagList->item(i);
        bool matches = text.isEmpty() || item->text().contains(text, Qt::CaseInsensitive);
        item->setHidden(!matches);
    }

    for (int i = 0; i < m_selectedTagList->count(); ++i) {
        QListWidgetItem* item = m_selectedTagList->item(i);
        bool matches = text.isEmpty() || item->text().contains(text, Qt::CaseInsensitive);
        item->setHidden(!matches);
    }
}

void TagManagerDialog::setDarkMode(bool dark)
{
    m_darkMode = dark;
    applyTheme();
}

QStringList TagManagerDialog::getTags(const QString& bundlePath,
                                      QWidget* parent,
                                      const QStringList& currentTags)
{
    TagManagerDialog dialog(bundlePath, parent);
    dialog.m_selectedTags = currentTags;
    dialog.populateTags();

    if (dialog.exec() == QDialog::Accepted) {
        return dialog.m_selectedTags;
    }
    return currentTags;
}

void TagManagerDialog::onSearchTextChanged(const QString& text)
{
    filterTags(text);
}

void TagManagerDialog::onTagClicked(QListWidgetItem* item)
{
    if (!item) return;

    QString tag = item->text();
    bool isInSelected = (m_selectedTagList->row(item) >= 0);

    if (isInSelected) {
        // Remove from selected - move to available
        int row = m_selectedTagList->row(item);
        m_selectedTagList->takeItem(row);
        m_selectedTags.removeAll(tag);

        // Add to available if not already there
        bool found = false;
        for (int i = 0; i < m_tagList->count(); ++i) {
            if (m_tagList->item(i)->text() == tag) {
                found = true;
                break;
            }
        }
        if (!found) {
            m_tagList->addItem(tag);
        }
    } else {
        // Add to selected - move to selected list
        int row = m_tagList->row(item);
        m_tagList->takeItem(row);
        m_selectedTags.append(tag);

        // Add to selected list
        m_selectedTagList->addItem(tag);
    }

    // Update delete button state
    m_deleteTagButton->setEnabled(m_selectedTagList->currentItem() != nullptr ||
                                   m_tagList->currentItem() != nullptr);
}

void TagManagerDialog::onAddTagClicked()
{
    bool ok;
    QString newTag = QInputDialog::getText(this, tr("Add Tag"),
        tr("Enter new tag name:"), QLineEdit::Normal, QString(), &ok);

    if (ok && !newTag.trimmed().isEmpty()) {
        newTag = newTag.trimmed();
        if (!m_allTags.contains(newTag, Qt::CaseInsensitive)) {
            m_allTags.append(newTag);
            m_allTags.sort(Qt::CaseInsensitive);
            m_tagList->addItem(newTag);
        }
        // Also add to selected
        if (!m_selectedTags.contains(newTag, Qt::CaseInsensitive)) {
            m_selectedTags.append(newTag);
            m_selectedTagList->addItem(newTag);

            // Remove from available list if present
            for (int i = 0; i < m_tagList->count(); ++i) {
                if (m_tagList->item(i)->text().compare(newTag, Qt::CaseInsensitive) == 0) {
                    delete m_tagList->takeItem(i);
                    break;
                }
            }
        }
    }
}

void TagManagerDialog::onDeleteTagClicked()
{
    QListWidgetItem* current = m_tagList->currentItem();
    if (!current) {
        current = m_selectedTagList->currentItem();
    }

    if (!current) return;

    QString tag = current->text();

    QMessageBox::StandardButton reply = QMessageBox::question(this,
        tr("Delete Tag"),
        tr("Are you sure you want to delete the tag \"%1\" from all notebooks?").arg(tag),
        QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        // Remove from all lists
        m_allTags.removeAll(tag);
        m_selectedTags.removeAll(tag);

        delete m_tagList->takeItem(m_tagList->row(current));
        delete m_selectedTagList->takeItem(m_selectedTagList->row(current));

        // TODO: Actually remove the tag from all notebooks in the library
        // This would require iterating through all notebooks and updating their document.json
    }
}

void TagManagerDialog::onApplyClicked()
{
    // Save tags to notebook if a bundle path is provided
    if (!m_bundlePath.isEmpty()) {
        saveTagsToNotebook(m_selectedTags);
    }
    accept();
}

void TagManagerDialog::onCancelClicked()
{
    reject();
}

void TagManagerDialog::saveTagsToNotebook(const QStringList& tags)
{
    QString docPath = m_bundlePath + "/document.json";
    QFile file(docPath);
    if (!file.open(QIODevice::ReadWrite)) {
        qWarning() << "TagManagerDialog: Cannot open" << docPath;
        return;
    }

    QJsonParseError parseError;
    QJsonDocument doc = QJsonDocument::fromJson(file.readAll(), &parseError);
    file.close();

    if (parseError.error != QJsonParseError::NoError || !doc.isObject()) {
        qWarning() << "TagManagerDialog: JSON parse error";
        return;
    }

    QJsonObject obj = doc.object();
    if (tags.isEmpty()) {
        obj.remove("tags");
    } else {
        obj["tags"] = QJsonArray::fromStringList(tags);
    }

    if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        qWarning() << "TagManagerDialog: Cannot write to" << docPath;
        return;
    }
    file.write(doc.toJson(QJsonDocument::Indented));
    file.close();

    // Reload notebook library to pick up changes
    NotebookLibrary::instance()->refreshNotebook(m_bundlePath);
}